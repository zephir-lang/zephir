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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\LiteralCompiledExpression;
use Zephir\Name;
use Zephir\Variable\Variable;

use function array_merge;
use function constant;
use function defined;
use function gettype;
use function in_array;
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
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
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
                    return new LiteralCompiledExpression('string', Name::addSlashes($constantName), $expression);

                case 'object':
                    throw new CompilerException('?');
                default:
                    return new LiteralCompiledExpression($type, $constantName, $expression);
            }
        }

        if (in_array($constantName, $this->magicConstants)) {
            switch ($constantName) {
                case '__CLASS__':
                    return new CompiledExpression(
                        'string',
                        Name::addSlashes($compilationContext->classDefinition->getCompleteName()),
                        $expression
                    );
                case '__NAMESPACE__':
                    return new CompiledExpression(
                        'string',
                        Name::addSlashes($compilationContext->classDefinition->getNamespace()),
                        $expression
                    );
                case '__METHOD__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->classDefinition->getName(
                        ) . ':' . $compilationContext->currentMethod->getName(),
                        $expression
                    );
                case '__FUNCTION__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->currentMethod->getName(),
                        $expression
                    );
            }

            $compilationContext->logger->warning(
                "Magic constant '" . $constantName . "' is not supported",
                ['not-supported-magic-constant', $expression]
            );

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
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, Variable $expectingVariable = null): void
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
