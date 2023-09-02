<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable;

/**
 * Resolves expressions that read array indexes
 */
class NativeArrayAccess
{
    protected bool $expecting = true;

    protected bool $readOnly = false;

    /** @var Variable|null */
    protected $expectingVariable;

    /** @var bool */
    protected $noisy = true;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool     $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->expecting = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Sets whether the expression must be resolved in "noisy" mode.
     *
     * @param bool $noisy
     */
    public function setNoisy($noisy)
    {
        $this->noisy = $noisy;
    }

    /**
     * Compiles foo[x] = {expr}.
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
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
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $compilationContext, $expression);
                switch ($variableVariable->getType()) {
                    case 'variable':
                    case 'array':
                    case 'string':
                        break;

                    default:
                        throw new CompilerException('Variable type: '.$variableVariable->getType().' cannot be used as array', $expression['left']);
                }
                break;

            default:
                throw new CompilerException('Cannot use expression: '.$exprVariable->getType().' as an array', $expression['left']);
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
     * @param array              $expression
     * @param Variable           $variableVariable
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    protected function accessStringOffset(array $expression, Variable $variableVariable, CompilationContext $compilationContext)
    {
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('char' != $symbolVariable->getType() && 'uchar' != $symbolVariable->getType()) {
                    $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('uchar', $compilationContext);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('uchar', $compilationContext);
            }
        }

        /**
         * Right part of expression is the index.
         */
        $expr = new Expression($expression['right']);
        $exprIndex = $expr->compile($compilationContext);

        $codePrinter = $compilationContext->codePrinter;
        $variableCode = $compilationContext->backend->getVariableCode($variableVariable);

        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $compilationContext->headersManager->add('kernel/operators');
                $codePrinter->output($symbolVariable->getName().' = ZEPHIR_STRING_OFFSET('.$variableCode.', '.$exprIndex->getCode().');');
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $expression);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $codePrinter->output($symbolVariable->getName().' = ZEPHIR_STRING_OFFSET('.$variableCode.', '.$variableIndex->getName().');');
                        break;

                    default:
                        throw new CompilerException('Cannot use index type '.$variableIndex->getType().' as offset', $expression['right']);
                }
                break;

            default:
                throw new CompilerException('Cannot use index type '.$exprIndex->getType().' as offset', $expression['right']);
        }

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }

    /**
     * @param array              $expression
     * @param Variable           $variableVariable
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    protected function accessDimensionArray($expression, Variable $variableVariable, CompilationContext $compilationContext)
    {
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
         * Resolves the symbol that expects the value.
         */
        $readOnly = false;
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
                        $numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
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
                        $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedUninitializedVariable('variable', $compilationContext, $expression);
                    }
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedUninitializedVariable('variable', $compilationContext, $expression);
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
                        $numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
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
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
                    }
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
            }
        }

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Cannot use variable: '.$symbolVariable->getType().' to assign array index', $expression);
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
        $expr = new Expression($arrayAccess['right']);
        $exprIndex = $expr->compile($compilationContext);
        $compilationContext->headersManager->add('kernel/array');

        if ('variable' === $exprIndex->getType()) {
            $exprIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $expression);
        }

        $compilationContext->backend->arrayFetch($symbolVariable, $variableVariable, $exprIndex, $flags, $arrayAccess, $compilationContext);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
