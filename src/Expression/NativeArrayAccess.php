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
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Variable\Variable;

/**
 * Resolves expressions that read array indexes
 */
class NativeArrayAccess
{
    protected bool      $expecting = true;
    protected ?Variable $expectingVariable;
    protected bool      $noisy     = true;
    protected bool      $readOnly  = false;

    /**
     * Compiles foo[x] = {expr}.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        /**
         * Resolve the left part of the expression.
         */
        $expr = new Expression($expression['left']);
        $expr->setReadOnly(true);
        $exprVariable = $expr->compile($compilationContext);

        /**
         * Only dynamic variables can be used as arrays
         */
        switch ($exprVariable->getType()) {
            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                    $exprVariable->getCode(),
                    $compilationContext,
                    $expression
                );
                switch ($variableVariable->getType()) {
                    case 'variable':
                    case 'array':
                    case 'string':
                        break;

                    default:
                        throw new CompilerException(
                            'Variable type: ' . $variableVariable->getType() . ' cannot be used as array',
                            $expression['left']
                        );
                }
                break;

            case 'string':
            case 'istring':
                /**
                 * A string literal source, PHP's `"abc"[1]`. Box it into a
                 * temp zval so the same offset path can read it.
                 */
                $literalVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $compilationContext->backend->assignString(
                    $literalVariable,
                    Name::addSlashes($exprVariable->getCode()),
                    $compilationContext
                );

                return $this->accessStringOffset($expression, $literalVariable, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot use expression: ' . $exprVariable->getType() . ' as an array',
                    $expression['left']
                );
        }

        /**
         * Resolve the dimension according to variable's type
         */
        switch ($variableVariable->getType()) {
            case 'array':
            case 'variable':
                return $this->accessDimensionArray($expression, $variableVariable, $compilationContext);

            case 'string':
                return $this->accessStringOffset($expression, $variableVariable, $compilationContext);
        }
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
     * Sets whether the expression must be resolved in "noisy" mode.
     */
    public function setNoisy(bool $noisy): void
    {
        $this->noisy = $noisy;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }

    /**
     * @throws Exception
     * @throws ReflectionException
     */
    protected function accessDimensionArray(
        array $expression,
        Variable $variableVariable,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $arrayAccess = $expression;

        if ('variable' == $variableVariable->getType()) {
            if ($variableVariable->hasAnyDynamicType('unknown')) {
                throw new CompilerException('Cannot use non-initialized variable as an array', $arrayAccess['left']);
            }

            /**
             * Trying to use a non-object dynamic variable as object
             */
            if ($variableVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to access array-index on a non-array dynamic variable',
                    ['non-array-access', $arrayAccess['left']]
                );
            }
        }

        /**
         * A native char destination cannot receive a zval, so the fetch goes
         * through a temp and only the byte is handed over afterwards. This is
         * the dynamic counterpart of what `accessStringOffset()` does for a
         * compile-time `string` source, and it keeps `char c; let c = s[i];`
         * working whether `s` was declared `string` or `var`.
         *
         * It has to happen before the symbol is resolved below, or the char
         * would be registered in the memory frame as if it were a zval.
         */
        $charDestination = null;
        if (
            null !== $this->expectingVariable
            && in_array($this->expectingVariable->getType(), ['char', 'uchar'], true)
        ) {
            $charDestination         = $this->expectingVariable;
            $this->expectingVariable = null;
        }

        /**
         * Resolves the symbol that expects the value.
         */
        $readOnly       = false;
        $symbolVariable = $this->expectingVariable;

        if ($this->readOnly) {
            if ($this->expecting && $this->expectingVariable) {
                /**
                 * If a variable is assigned once in the method, we try to promote it
                 * to a read only variable
                 */
                if ('return_value' != $symbolVariable->getName()) {
                    $line = $compilationContext->symbolTable->getLastCallLine();
                    if (false === $line || ($line > 0 && $line < $expression['line'])) {
                        $numberMutations = $compilationContext->symbolTable->getExpectedMutations(
                            $symbolVariable->getName()
                        );
                        if (1 == $numberMutations) {
                            if ($symbolVariable->getNumberMutations() == $numberMutations) {
                                $symbolVariable->setMemoryTracked(false);
                                $readOnly = true;
                            }
                        }
                    }
                }

                /**
                 * Variable is not read-only or it wasn't promoted
                 */
                if (!$readOnly) {
                    if ('return_value' != $symbolVariable->getName()) {
                        $symbolVariable->observeVariant($compilationContext);
                        $this->readOnly = false;
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedUninitializedVariable(
                            'variable',
                            $compilationContext,
                        );
                    }
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedUninitializedVariable(
                    'variable',
                    $compilationContext,
                );
            }
        } else {
            if ($this->expecting && $this->expectingVariable) {
                /**
                 * If a variable is assigned once in the method, we try to promote it
                 * to a read only variable
                 */
                if ('return_value' !== $symbolVariable->getName()) {
                    $line = $compilationContext->symbolTable->getLastCallLine();
                    if (false === $line || ($line > 0 && $line < $expression['line'])) {
                        $numberMutations = $compilationContext->symbolTable->getExpectedMutations(
                            $symbolVariable->getName()
                        );
                        if (1 == $numberMutations) {
                            if ($symbolVariable->getNumberMutations() == $numberMutations) {
                                $symbolVariable->setMemoryTracked(false);
                                $readOnly = true;
                            }
                        }
                    }
                }

                /**
                 * Variable is not read-only or it wasn't promoted
                 */
                if (!$readOnly) {
                    if ('return_value' != $symbolVariable->getName()) {
                        $symbolVariable->observeVariant($compilationContext);
                        $this->readOnly = false;
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve(
                            'variable',
                            $compilationContext
                        );
                    }
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve(
                    'variable',
                    $compilationContext
                );
            }
        }

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable: ' . $symbolVariable->getType() . ' to assign array index',
                $expression
            );
        }

        /**
         * At this point, we don't know the type fetched from the index
         */
        $symbolVariable->setDynamicTypes('undefined');

        if ($this->readOnly || $readOnly) {
            $flags = $this->noisy ? 'PH_NOISY | PH_READONLY' : 'PH_READONLY';
        } else {
            $flags = $this->noisy ? 'PH_NOISY' : '0';
        }

        /**
         * Right part of expression is the index.
         */
        $expr      = new Expression($arrayAccess['right']);
        $exprIndex = $expr->compile($compilationContext);
        $compilationContext->headersManager->add('kernel/array');

        if ('variable' === $exprIndex->getType()) {
            $exprIndex = $compilationContext->symbolTable->getVariableForRead(
                $exprIndex->getCode(),
                $compilationContext,
                $expression
            );
        }

        $compilationContext->backend->arrayFetch(
            $symbolVariable,
            $variableVariable,
            $exprIndex,
            $flags,
            $arrayAccess,
            $compilationContext
        );

        if (null !== $charDestination) {
            $compilationContext->headersManager->add('kernel/operators');
            $compilationContext->codePrinter->output(sprintf(
                '%s = (unsigned char) zephir_get_charval(%s);',
                $charDestination->getName(),
                $compilationContext->backend->getVariableCode($symbolVariable)
            ));

            return new CompiledExpression('variable', $charDestination->getName(), $expression);
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @throws ReflectionException
     * @throws Exception
     */
    protected function accessStringOffset(
        array $expression,
        Variable $variableVariable,
        CompilationContext $compilationContext
    ): CompiledExpression {
        /**
         * A string offset read is PHP's `$s[$i]`, whose value is a 1-char string.
         * When the destination is a zval we must box it as one; when it is a
         * native C scalar we write the raw byte straight in. See #1629.
         */
        $boxAsString = $this->needsStringBoxing($compilationContext);

        if ($this->expecting) {
            if ($this->expectingVariable && !$boxAsString) {
                $symbolVariable = $this->expectingVariable;
                if ('char' != $symbolVariable->getType() && 'uchar' != $symbolVariable->getType()) {
                    $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                        'uchar',
                        $compilationContext
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'uchar',
                    $compilationContext
                );
            }
        }

        /**
         * Right part of expression is the index.
         */
        $expr      = new Expression($expression['right']);
        $exprIndex = $expr->compile($compilationContext);

        $codePrinter  = $compilationContext->codePrinter;
        $variableCode = $compilationContext->backend->getVariableCode($variableVariable);
        $offset       = $compilationContext->backend->resolveStringOffset(
            $exprIndex,
            $compilationContext,
            $expression['right']
        );

        /**
         * PH_NOISY is the kernel's equivalent of PHP's BP_VAR_R: warn on an
         * out-of-range offset and yield "". Without it the read is the
         * BP_VAR_IS one an `isset`/`fetch` guard performs: silent, yielding
         * null.
         */
        $flags = $this->noisy ? 'PH_NOISY' : '0';

        $compilationContext->headersManager->add('kernel/string');

        if ($boxAsString) {
            $stringVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );

            $codePrinter->output(sprintf(
                'zephir_string_offset_read%s(%s, %s, %s, %s);',
                $offset['suffix'],
                $compilationContext->backend->getVariableCode($stringVariable),
                $variableCode,
                $offset['code'],
                $flags
            ));

            return new CompiledExpression('variable', $stringVariable->getName(), $expression);
        }

        $codePrinter->output(sprintf(
            '%s = zephir_string_offset_byte%s(%s, %s, %s);',
            $symbolVariable->getName(),
            $offset['suffix'],
            $variableCode,
            $offset['code'],
            $flags
        ));

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }

    /**
     * Whether the byte read out of the string has to be boxed into a 1-char
     * string zval rather than handed over as a raw C byte.
     *
     * It does when the destination is dynamic (`var`) or `string`, and when
     * there is no destination variable at all -- `let a[] = s[i]`, `f(s[i])`,
     * `echo s[i]`. It does not for a native scalar destination (`char`, `int`,
     * `double`, ...), which wants the byte.
     *
     * `return_value` is decided by the declared return type: a method
     * declaring `-> char` or `-> int` carries an `IS_LONG` arg-info, so
     * returning a 1-char string there would be a TypeError. An untyped return,
     * or one that admits `string`, gets the string PHP would have produced.
     */
    private function needsStringBoxing(CompilationContext $compilationContext): bool
    {
        if (!$this->expecting) {
            return false;
        }

        if (null === $this->expectingVariable) {
            return true;
        }

        if ('return_value' === $this->expectingVariable->getName()) {
            $method = $compilationContext->currentMethod;

            if (null === $method || !$method->hasReturnTypes()) {
                return true;
            }

            return !$method->areReturnTypesIntCompatible() || $method->areReturnTypesStringCompatible();
        }

        return in_array(
            $this->expectingVariable->getType(),
            ['variable', 'mixed', 'string', 'istring'],
            true
        );
    }
}
