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

use ReflectionException;
use Zephir\Class\Constant;
use Zephir\Class\Definition\AbstractDefinition;
use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Variable\Variable;

use function gettype;
use function in_array;
use function sprintf;

/**
 * Resolves class constants
 */
class StaticConstantAccess
{
    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $readOnly          = false;

    /**
     * Access a static constant class.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $className = $expression['left']['value'];
        $constant  = $expression['right']['value'];

        if (!in_array($className, ['this', 'self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
        }

        /**
         * A parent only known at runtime still has a known name, which is all
         * `parent::class` needs. See #2714.
         */
        $parentName = $compilationContext->classDefinition?->getExtendsClass();
        if (
            'class' === $constant
            && 'parent' === $className
            && $parentName
            && !$compilationContext->classDefinition->getExtendsClassDefinition() instanceof Definition
        ) {
            return new CompiledExpression('string', Name::addSlashes($parentName), $expression);
        }

        $classDefinition = $this->resolveClassDefinition($className, $expression, $compilationContext);

        /**
         * The `::class` postfix is not a real constant: it resolves to the
         * fully-qualified class name. `self`/`parent`/explicit names fold to a
         * compile-time string literal (exactly like `__CLASS__`); `static::class`
         * needs runtime late static binding via the called scope. See #2527.
         */
        if ('class' === $constant) {
            if ('static' === $className) {
                if ($this->expecting && $this->expectingVariable) {
                    $symbolVariable = $this->expectingVariable;
                    $symbolVariable->initVariant($compilationContext);
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                }

                $symbolVariable->setDynamicTypes('string');
                $compilationContext->headersManager->add('kernel/object');
                $compilationContext->codePrinter->output(
                    'zephir_get_called_class(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');'
                );

                return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
            }

            return new CompiledExpression(
                'string',
                Name::addSlashes($classDefinition->getCompleteName()),
                $expression
            );
        }

        /**
         * A version-shaped engine constant is emitted as its C macro, before
         * the existence check: the PHP running Zephir may not have it at all
         * (`Attribute::TARGET_CONSTANT` before 8.5). See #2738.
         */
        $native = $this->nativeOf($className, $classDefinition, $constant, $compilationContext);
        if (null !== $native) {
            return new CompiledExpression('int', $native['c'], $expression);
        }

        /**
         * Constants are resolved to their values at compile time,
         * so we need to check that they effectively do exist
         */
        if (!$classDefinition->hasConstant($constant)) {
            throw new CompilerException(
                sprintf(
                    "Class '%s' does not have a constant called: '%s'",
                    $classDefinition->getCompleteName(),
                    $constant
                ),
                $expression
            );
        }

        /**
         * We can optimize the reading of constants by avoiding query their value every time
         */
        if (!$compilationContext->config->get('static-constant-class-folding', 'optimizations')) {
            /**
             * Resolves the symbol that expects the value
             */
            if ($this->expecting) {
                if ($this->expectingVariable) {
                    $symbolVariable = $this->expectingVariable;
                    $symbolVariable->initVariant($compilationContext);
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }

            /**
             * Variable that receives property accesses must be polymorphic
             */
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException(
                    'Cannot use variable: ' . $symbolVariable->getType() . ' to assign class constants',
                    $expression
                );
            }

            $symbolVariable->setDynamicTypes('undefined');
            $compilationContext->headersManager->add('kernel/object');
            /**
             * The destination is a `zval *` parameter, so it goes through
             * getVariableCode() like every other emitter here. A bare name
             * hands over the zval itself and the generated C does not compile.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2711
             */
            $compilationContext->codePrinter->output(
                sprintf(
                    'zephir_get_class_constant(%s, %s, SL("%s"));',
                    $compilationContext->backend->getVariableCode($symbolVariable),
                    $classDefinition->getClassEntry($compilationContext),
                    $constant
                )
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $constantDefinition = $classDefinition->getConstant($constant);

        /**
         * Array constants cannot be folded to a scalar literal; compile the
         * constant's array value as a fresh array expression at the use site.
         */
        if (
            $constantDefinition instanceof Constant &&
            in_array($constantDefinition->getValueType(), ['array', 'empty-array'], true)
        ) {
            $expression = new Expression($constantDefinition->getValue());
            $expression->setExpectReturn($this->expecting, $this->expectingVariable);
            $expression->setReadOnly($this->readOnly);

            return $expression->compile($compilationContext);
        }

        if ($constantDefinition instanceof Constant) {
            $constantDefinition->processValue($compilationContext);
            $value = $constantDefinition->getValueValue();
            $type  = $constantDefinition->getValueType();
        } else {
            $value = $constantDefinition;
            $type  = gettype($value);
            if ('integer' === $type) {
                $type = 'int';
            }
        }

        switch ($type) {
            case 'string':
            case 'int':
            case 'double':
            case 'float':
            case 'bool':
            case 'null':
                break;
            default:
                $compilationContext->logger->warning(
                    "Constant '" . $constantDefinition->getName() . "' does not exist at compile time",
                    ['nonexistent-constant', $expression]
                );

                return new CompiledExpression('null', null, $expression);
        }

        return new CompiledExpression($type, $value, $expression);
    }

    /**
     * The C and fully-qualified PHP spellings of a constant whose value depends
     * on the PHP version that compiles the C, or null for any other constant.
     *
     * That is an `Attribute::*` flag, or a Zephir class constant initialized
     * from one, so `self::FLAGS` keeps the macro of what it was declared as.
     *
     * @return array{c: string, qualified: string}|null
     *
     * @throws Exception
     * @throws ReflectionException
     *
     * @see https://github.com/zephir-lang/zephir/issues/2738
     */
    public function resolveNative(array $expression, CompilationContext $compilationContext): ?array
    {
        $className = $expression['left']['value'];
        $constant  = $expression['right']['value'];

        if ('class' === $constant) {
            return null;
        }

        if (!in_array($className, ['this', 'self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
        }

        /**
         * A class this lookup cannot see is simply not native. The leaf may
         * sit in a branch the evaluator never reaches, and compile() reports
         * it wherever it is actually read.
         */
        if (!$this->isResolvable($className, $compilationContext)) {
            return null;
        }

        return $this->nativeOf(
            $className,
            $this->resolveClassDefinition($className, $expression, $compilationContext),
            $constant,
            $compilationContext
        );
    }

    /**
     * @return array{c: string, qualified: string}|null
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function nativeOf(
        string $className,
        AbstractDefinition $classDefinition,
        string $constant,
        CompilationContext $compilationContext
    ): ?array {
        $macro = AttributeConstants::macro($className, $constant);
        if (null !== $macro) {
            return ['c' => $macro, 'qualified' => '\\Attribute::' . $constant];
        }

        if (!$classDefinition->hasConstant($constant)) {
            return null;
        }

        $constantDefinition = $classDefinition->getConstant($constant);
        if (!$constantDefinition instanceof Constant) {
            return null;
        }

        $constantDefinition->processValue($compilationContext);
        $value = $constantDefinition->getValue();

        if (!isset($value['native'])) {
            return null;
        }

        return [
            'c'         => '(' . $value['value'] . ')',
            'qualified' => '\\' . $classDefinition->getCompleteName() . '::' . $constant,
        ];
    }

    /**
     * Whether resolveClassDefinition() would find a definition for $className.
     */
    private function isResolvable(string $className, CompilationContext $compilationContext): bool
    {
        $compiler = $compilationContext->compiler;

        if (in_array($className, ['self', 'static', 'this'])) {
            return null !== $compilationContext->classDefinition;
        }

        if ('parent' === $className) {
            return $compilationContext->classDefinition?->getExtendsClassDefinition() instanceof Definition;
        }

        return $compiler->isClass($className)
            || $compiler->isInterface($className)
            || $compiler->isBundledClass($className)
            || $compiler->isBundledInterface($className);
    }

    /**
     * Fetches the class definition according to the class where the constant
     * is supposed to be declared.
     *
     * @param string $className resolved name, or one of this/self/static/parent
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function resolveClassDefinition(
        string $className,
        array $expression,
        CompilationContext $compilationContext
    ): AbstractDefinition {
        $compiler = $compilationContext->compiler;

        if (in_array($className, ['self', 'static', 'this'])) {
            return $compilationContext->classDefinition;
        }

        if ('parent' === $className) {
            return $compilationContext->parentClassDefinition(
                sprintf('find constant called "%s"', $expression['right']['value']),
                $expression
            );
        }

        if ($compiler->isClass($className) || $compiler->isInterface($className)) {
            return $compiler->getClassDefinition($className);
        }

        if ($compiler->isBundledClass($className) || $compiler->isBundledInterface($className)) {
            return $compiler->getInternalClassDefinition($className);
        }

        throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
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
