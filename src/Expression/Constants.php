<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Expression;

use Zephir\Class\Entry;
use Zephir\Class\Method\Method;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\FunctionDefinition;
use Zephir\LiteralCompiledExpression;
use Zephir\Name;
use Zephir\Variable\Variable;

use function array_merge;
use function constant;
use function defined;
use function gettype;
use function in_array;
use function sprintf;
use function strtolower;

/**
 * Resolves PHP or Zephir constants into C-Code
 */
class Constants
{
    /**
     * Reserved ENV Constants.
     *
     * @see https://www.php.net/manual/ru/reserved.constants.php
     */
    protected array     $envConstants      = [
        'PHP_VERSION',
        'PHP_MAJOR_VERSION',
        'PHP_MINOR_VERSION',
        'PHP_RELEASE_VERSION',
        'PHP_VERSION_ID',
        'PHP_EXTRA_VERSION',
        'PHP_ZTS',
        'PHP_DEBUG',
        'PHP_MAXPATHLEN',
        'PHP_OS',
        'PHP_SAPI',
        'PHP_EOL',
        'PHP_INT_MAX',
        'PHP_INT_SIZE',
        'DEFAULT_INCLUDE_PATH',
        'PHP_BINDIR',
        'PHP_LIBDIR',
        'PHP_DATADIR',
    ];
    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;

    /**
     * Magic constants.
     *
     * @see https://php.net/manual/en/language.constants.predefined.php
     */
    protected array $magicConstants = [
        '__LINE__',
        '__FILE__',
        '__DIR__',
        '__FUNCTION__',
        '__CLASS__',
        '__TRAIT__',
        '__METHOD__',
        '__NAMESPACE__',
    ];
    protected bool  $readOnly       = false;
    protected array $resources      = [
        'STDIN',
        'STDOUT',
        'STDERR',
    ];

    /**
     * Resolves a PHP constant value into C-code.
     *
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $isPhpConstant    = false;
        $isZephirConstant = false;

        $constantName = $expression['value'];

        $mergedConstants = array_merge($this->envConstants, $this->magicConstants, $this->resources);
        if (!defined($expression['value']) && !in_array($constantName, $mergedConstants)) {
            if (!$compilationContext->compiler->isConstant($constantName)) {
                $compilationContext->logger->warning(
                    "Constant '" . $constantName . "' does not exist at compile time",
                    ['nonexistent-constant', $expression]
                );
            } else {
                $isZephirConstant = true;
            }
        } else {
            $isPhpConstant = !str_contains($constantName, 'VERSION');
        }

        if ($isZephirConstant && !in_array($constantName, $this->resources)) {
            $constant = $compilationContext->compiler->getConstant($constantName);

            return new LiteralCompiledExpression($constant[0], $constant[1], $expression);
        }

        if ($isPhpConstant && !in_array($constantName, $mergedConstants)) {
            $constantName = constant($constantName);
            $type         = strtolower(gettype($constantName));

            switch ($type) {
                case 'integer':
                    return new LiteralCompiledExpression('int', $constantName, $expression);

                case 'double':
                    return new LiteralCompiledExpression('double', $constantName, $expression);

                case 'string':
                    return new LiteralCompiledExpression('string', Name::encodeSource($constantName), $expression);

                case 'boolean':
                    // gettype() names a bool "boolean", a type no consumer knows (#2771).
                    return new LiteralCompiledExpression('bool', $constantName ? 'true' : 'false', $expression);

                case 'object':
                    throw new CompilerException(
                        "PHP constant '" . $expression['value'] . "' resolves to an "
                        . 'object, which cannot be used as a compile-time constant',
                        $expression
                    );
                default:
                    return new LiteralCompiledExpression($type, $constantName, $expression);
            }
        }

        if (in_array($constantName, $this->magicConstants)) {
            if (
                in_array($constantName, ['__FUNCTION__', '__METHOD__'], true)
                && true === $compilationContext->classDefinition?->isClosure()
                && null !== $compilationContext->symbolTable
            ) {
                return $this->compileClosureName($expression, $compilationContext);
            }

            $name = $this->magicConstantValue($constantName, $compilationContext);
            if (null !== $name) {
                return new CompiledExpression('string', Entry::escape($name), $expression);
            }

            $compilationContext->logger->warning(
                "Magic constant '" . $constantName . "' is not supported",
                ['not-supported-magic-constant', $expression]
            );

            return new LiteralCompiledExpression('null', null, $expression);
        }

        /**
         * A constant initializer compiled outside an executable scope (a
         * class/interface constant declaration) has no symbol table, so the
         * runtime `ZEPHIR_GET_CONSTANT` path below is unavailable. Fold a
         * defined PHP constant to its literal value instead.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2542
         */
        if (null === $compilationContext->symbolTable) {
            if (defined($constantName)) {
                $value = constant($constantName);

                return match (strtolower(gettype($value))) {
                    'integer' => new LiteralCompiledExpression('int', $value, $expression),
                    'double'  => new LiteralCompiledExpression('double', $value, $expression),
                    'boolean' => new LiteralCompiledExpression('bool', $value ? 'true' : 'false', $expression),
                    'string'  => new LiteralCompiledExpression('string', Name::encodeSource($value), $expression),
                    'null'    => new LiteralCompiledExpression('null', null, $expression),
                    default   => new LiteralCompiledExpression('null', null, $expression),
                };
            }

            return new LiteralCompiledExpression('null', null, $expression);
        }

        if ($this->expecting && $this->expectingVariable) {
            $symbolVariable = $this->expectingVariable;

            $symbolVariable->setLocalOnly(false);
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->initVariant($compilationContext);
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable: '
                . $symbolVariable->getType()
                . ' to assign property value',
                $expression
            );
        }

        $compilationContext->codePrinter->output(
            'ZEPHIR_GET_CONSTANT(' . $compilationContext->backend->getVariableCode(
                $symbolVariable
            ) . ', "' . $expression['value'] . '");'
        );

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }

    /**
     * The unescaped value PHP folds a magic constant to, or null for one
     * Zephir does not support.
     *
     * `__METHOD__` names the class or trait the method is written in, a
     * function is named with its namespace, and a declaration outside any
     * function (a class constant, a property default) gets "".
     *
     * @see https://github.com/zephir-lang/zephir/issues/2825
     */
    private function magicConstantValue(string $name, CompilationContext $compilationContext): ?string
    {
        $method = null === $compilationContext->symbolTable ? null : $compilationContext->currentMethod;
        $class  = $compilationContext->lexicalClassDefinition();

        return match ($name) {
            '__CLASS__'     => $class?->getCompleteName() ?? '',
            '__NAMESPACE__' => $method instanceof FunctionDefinition && $method->isGlobal()
                ? ''
                : $compilationContext->currentNamespace(),
            '__TRAIT__'     => $this->declaringTrait($compilationContext, $method) ?? '',
            '__FUNCTION__'  => $method instanceof FunctionDefinition
                ? $this->functionName($method)
                : $method?->getDeclaredName() ?? '',
            '__METHOD__'    => match (true) {
                null === $method                     => '',
                $method instanceof FunctionDefinition => $this->functionName($method),
                default                              => (
                    $this->declaringTrait($compilationContext, $method) ?? $class->getCompleteName()
                ) . '::' . $method->getDeclaredName(),
            },
            default         => null,
        };
    }

    /**
     * The trait the code is written in: the trait being compiled, or the one
     * a method merged into a class came from.
     */
    private function declaringTrait(CompilationContext $compilationContext, ?Method $method): ?string
    {
        $merged = $method?->getExpression()['declaring-trait'] ?? null;
        if (null !== $merged) {
            return $merged;
        }

        $class = $compilationContext->classDefinition;
        if (true === $class?->isClosure()) {
            return $class->getClosureTrait();
        }

        return true === $class?->isTrait() ? $class->getCompleteName() : null;
    }

    /**
     * `__FUNCTION__` and `__METHOD__` in a closure are its name, which depends
     * on the PHP version the extension is built for. See #2825.
     */
    private function compileClosureName(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        if ($this->expecting && $this->expectingVariable?->isVariable()) {
            $symbolVariable = $this->expectingVariable;
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        [$legacyName, $modernName] = $compilationContext->classDefinition->getClosureNames();

        $symbolVariable->initVariant($compilationContext);
        $symbolVariable->setDynamicTypes('string');
        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_closure_name(%s, SL("%s"), SL("%s"));',
                $compilationContext->backend->getVariableCode($symbolVariable),
                Entry::escape($legacyName),
                Entry::escape($modernName)
            )
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * PHP's name of a function: qualified unless declared outside a namespace.
     */
    private function functionName(FunctionDefinition $function): string
    {
        return $function->isGlobal()
            ? $function->getName()
            : $function->getNamespace() . '\\' . $function->getName();
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }
}
