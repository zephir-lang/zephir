<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use function Zephir\add_slashes;
use Zephir\CodePrinter;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;
use Zephir\Variable as ZephirVariable;

/**
 * Zephir\Statements\Let\Variable.
 *
 * Assign a value to a variable.
 */
class Variable
{
    /**
     * Compiles foo = {expr}
     * Changes the value of a mutable variable.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param ReadDetector       $readDetector
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign(
        string $variable,
        ZephirVariable $symbolVariable,
        CompiledExpression $resolvedExpr,
        ReadDetector $readDetector,
        CompilationContext $compilationContext,
        array $statement
    ) {
        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is read only", $statement);
        }

        $codePrinter = $compilationContext->codePrinter;

        /*
         * Only initialize variables if it's direct assignment
         */
        if ('assign' == $statement['operator']) {
            $symbolVariable->setIsInitialized(true, $compilationContext);
        } else {
            if (!$symbolVariable->isInitialized()) {
                throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
            }
        }

        /*
         * Set the assigned value to the variable as a CompiledExpression
         * We could use this expression for further analysis
         */
        $symbolVariable->setPossibleValue($resolvedExpr, $compilationContext);

        $type = $symbolVariable->getType();
        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'char':
            case 'uchar':
                $this->doNumericAssignment($codePrinter, $resolvedExpr, $variable, $statement, $compilationContext);
                break;

            case 'double':
                $this->doDoubleAssignment($codePrinter, $resolvedExpr, $variable, $statement, $compilationContext);
                break;

            case 'array':
                $this->doArrayAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'string':
                $this->doStringAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'bool':
                $this->doBoolAssignment($codePrinter, $resolvedExpr, $variable, $statement, $compilationContext);
                break;

            case 'variable':
                $this->doVariableAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext, $readDetector);
                break;

            default:
                throw new CompilerException('Unknown type: '.$type, $statement);
        }
    }

    /**
     * Performs numeric assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws \Zephir\Exception\IllegalOperationException
     */
    private function doNumericAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ) {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = 0;');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += 0;');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= 0;');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= 0;');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = '.$resolvedExpr->getCode().';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += '.$resolvedExpr->getCode().';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= '.$resolvedExpr->getCode().';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= '.$resolvedExpr->getCode().';');
                        break;

                    case 'div-assign':
                        $codePrinter->output($variable.' /= '.$resolvedExpr->getCode().';');
                        break;

                    case 'mod-assign':
                        $codePrinter->output($variable.' %= '.$resolvedExpr->getCode().';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = \''.$resolvedExpr->getCode().'\';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += \''.$resolvedExpr->getCode().'\';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= \''.$resolvedExpr->getCode().'\';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= \''.$resolvedExpr->getCode().'\';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'double':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = (long) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += (long) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= (long) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= (long) ('.$resolvedExpr->getCode().');');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = '.$resolvedExpr->getBooleanCode().';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += '.$resolvedExpr->getBooleanCode().';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= '.$resolvedExpr->getBooleanCode().';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'bool':
                    case 'char':
                    case 'uchar':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = '.$itemVariable->getName().';');
                                break;

                            case 'add-assign':
                                $codePrinter->output($variable.' += '.$itemVariable->getName().';');
                                break;

                            case 'sub-assign':
                                $codePrinter->output($variable.' -= '.$itemVariable->getName().';');
                                break;

                            case 'mul-assign':
                                $codePrinter->output($variable.' *= '.$itemVariable->getName().';');
                                break;

                            case 'div-assign':
                                $codePrinter->output($variable.' /= '.$itemVariable->getName().';');
                                break;

                            case 'mod-assign':
                                $codePrinter->output($variable.' %= '.$itemVariable->getName().';');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = (long) '.$itemVariable->getName().';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable.' += (long) '.$itemVariable->getName().';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable.' -= (long) '.$itemVariable->getName().';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable.' *= (long) '.$itemVariable->getName().';');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $exprVariable = $compilationContext->symbolTable->getVariableForWrite($resolvedExpr->resolve(null, $compilationContext), $compilationContext);
                        $exprVariableCode = $compilationContext->backend->getVariableCode($exprVariable);
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = zephir_get_numberval('.$exprVariableCode.');');
                                break;

                            case 'add-assign':
                                $codePrinter->output($variable.' += zephir_get_numberval('.$exprVariableCode.');');
                                break;

                            case 'sub-assign':
                                $codePrinter->output($variable.' -= zephir_get_numberval('.$exprVariableCode.');');
                                break;

                            case 'mul-assign':
                                $codePrinter->output($variable.' *= zephir_get_numberval('.$exprVariableCode.');');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: '.$itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException("Value type '".$resolvedExpr->getType()."' cannot be assigned to variable: int", $statement);
        }
    }

    /**
     * Performs double assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doDoubleAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ) {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = 0.0;');
                        break;
                    case 'add-assign':
                        $codePrinter->output($variable.' += 0.0;');
                        break;
                    case 'sub-assign':
                        $codePrinter->output($variable.' -= 0.0;');
                        break;
                    case 'mul-assign':
                        $codePrinter->output($variable.' *= 0.0;');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = (double) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += (double) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= (double) ('.$resolvedExpr->getCode().');');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= (double) ('.$resolvedExpr->getCode().');');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'double':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = '.$resolvedExpr->getCode().';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += '.$resolvedExpr->getCode().';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= '.$resolvedExpr->getCode().';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= '.$resolvedExpr->getCode().';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = '.$resolvedExpr->getBooleanCode().';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable.' += '.$resolvedExpr->getBooleanCode().';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable.' -= '.$resolvedExpr->getBooleanCode().';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable.' *= '.$resolvedExpr->getBooleanCode().';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = (double) '.$itemVariable->getName().';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable.' += (double) '.$itemVariable->getName().';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable.' -= (double) '.$itemVariable->getName().';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable.' *= (double) '.$itemVariable->getName().';');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = '.$itemVariable->getName().';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable.' += '.$itemVariable->getName().';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable.' -= '.$itemVariable->getName().';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable.' *= '.$itemVariable->getName().';');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $exprVariableCode = $compilationContext->backend->getVariableCode($itemVariable);
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = zephir_get_numberval('.$exprVariableCode.');');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable.' += zephir_get_numberval('.$exprVariableCode.');');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable.' -= zephir_get_numberval('.$exprVariableCode.');');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable.' *= zephir_get_numberval('.$exprVariableCode.');');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: '.$itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException('Unknown type '.$resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs string assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param ZephirVariable     $symbolVariable
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws \Zephir\Exception\IllegalOperationException
     */
    private function doStringAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ) {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $compilationContext->backend->assignString($symbolVariable, null, $compilationContext);
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $compilationContext->backend->assignString($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        break;
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&'.$variable.', "'.$resolvedExpr->getCode().'", sizeof("'.$resolvedExpr->getCode().'") - 1);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'string':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        if ($resolvedExpr->getCode()) {
                            $compilationContext->backend->assignString($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        } else {
                            $compilationContext->backend->assignString($symbolVariable, null, $compilationContext);
                        }
                        break;
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&'.$variable.', "'.$resolvedExpr->getCode().'", sizeof("'.$resolvedExpr->getCode().'") - 1);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        if ($resolvedExpr->getCode()) {
                            $compilationContext->backend->assignString($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        } else {
                            $compilationContext->backend->assignString($symbolVariable, null, $compilationContext);
                        }
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output('zephir_concat_self_str(&'.$variable.', "'.$resolvedExpr->getCode().'", sizeof("'.$resolvedExpr->getCode().'") - 1);');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $compilationContext->headersManager->add('kernel/string');
                                // FIXME: Most likely this code is outdated and no longer works.
                                $codePrinter->output('Z_STRLEN_P('.$variable.') = zephir_spprintf(&Z_STRVAL_P('.$variable.'), 0, "%ld", '.$itemVariable->getName().');');
                                $codePrinter->output('Z_TYPE_P('.$variable.') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self_long(&'.$variable.', '.$itemVariable->getName().');');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'char':
                    case 'uchar':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $compilationContext->headersManager->add('kernel/string');
                                // FIXME: Most likely this code is outdated and no longer works.
                                $codePrinter->output('Z_STRLEN_P('.$variable.') = zephir_spprintf(&Z_STRVAL_P('.$variable.'), 0, "%c", '.$itemVariable->getName().');');
                                $codePrinter->output('Z_TYPE_P('.$variable.') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self_char(&'.$variable.', '.$itemVariable->getName().');');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->setMustInitNull(true);
                                $symbolVariable->increaseVariantIfNull();
                                $compilationContext->symbolTable->mustGrownStack(true);
                                if ($variable != $itemVariable->getName()) {
                                    $compilationContext->backend->copyOnWrite($symbolVariable, $itemVariable, $compilationContext);
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf($symbolVariable, $itemVariable, $compilationContext);
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->setMustInitNull(true);
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_get_strval('.$compilationContext->backend->getVariableCode($symbolVariable).', '.$compilationContext->backend->getVariableCode($itemVariable).');');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf($symbolVariable, $itemVariable, $compilationContext);
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: '.$itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type '.$resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs array assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param ZephirVariable     $symbolVariable
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doArrayAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ) {
        switch ($resolvedExpr->getType()) {
            case 'variable':
            case 'array':
                switch ($statement['operator']) {
                    case 'assign':
                        if ($variable != $resolvedExpr->getCode()) {
                            $symbolVariable->setMustInitNull(true);
                            $compilationContext->symbolTable->mustGrownStack(true);

                            /* Inherit the dynamic type data from the assigned value */
                            $symbolVariable->setDynamicTypes('array');
                            $symbolVariable->increaseVariantIfNull();
                            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                            $codePrinter->output('ZEPHIR_CPY_WRT('.$symbol.', '.$compilationContext->backend->resolveValue($resolvedExpr, $compilationContext).');');
                        }
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException("Cannot '".$statement['operator']."' ".$resolvedExpr->getType().' for array type', $resolvedExpr->getOriginal());
        }
    }

    /**
     * Performs boolean assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doBoolAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ) {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = 0;');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = (('.$resolvedExpr->getCode().') ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'double':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = (('.$resolvedExpr->getCode().' != 0.0) ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = ((\''.$resolvedExpr->getCode().'\') ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable.' = '.$resolvedExpr->getBooleanCode().';');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = (('.$itemVariable->getName().') ? 1 : 0);');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = (('.$itemVariable->getName().' != 0.0) ? 1 : 0);');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable.' = '.$itemVariable->getName().';');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                    case 'string':
                    case 'array':
                        switch ($statement['operator']) {
                            case 'assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output($variable.' = zephir_is_true('.$compilationContext->backend->getVariableCode($itemVariable).');');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Cannot assign variable: '.$itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type: '.$resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs variable assignment.
     *
     * @param CodePrinter        $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param ZephirVariable     $symbolVariable
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     * @param ReadDetector       $readDetector
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doVariableAssignment(
        CodePrinter $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext,
        ReadDetector $readDetector
    ) {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $symbolVariable->setDynamicTypes('null');

                        $compilationContext->backend->assignNull($symbolVariable, $compilationContext);
                        break;
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':
                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;

                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;

                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);

                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output($functionName.'('.$symbol.', '.$compilationContext->backend->getVariableCode($tempVariable).');');
                        break;

                    case 'assign':
                        $symbolVariable->setDynamicTypes('long');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('int', $compilationContext);
                            $codePrinter->output($tempVariable->getName().' = '.$resolvedExpr->getCode().';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong($symbolVariable, $tempVariable, $compilationContext);
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        }
                        break;

                    case 'div-assign':
                        $symbolVariable->setDynamicTypes('double');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                            $codePrinter->output($tempVariable->getName().' = '.$resolvedExpr->getCode().';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignDouble($symbolVariable, $tempVariable, $compilationContext);
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignDouble($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        }
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->setDynamicTypes('long');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
                            $codePrinter->output($tempVariable->getName().' = '.$resolvedExpr->getCode().';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong($symbolVariable, $tempVariable, $compilationContext);
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong($symbolVariable, '\''.$resolvedExpr->getCode().'\'', $compilationContext);
                        }

                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'double':
                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':
                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;

                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;

                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $tempVariable->setDynamicTypes('double');
                        $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getCode(), $compilationContext);

                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output($functionName.'('.$compilationContext->backend->getVariableCode($symbolVariable).', '.$compilationContext->backend->getVariableCode($tempVariable).');');
                        break;

                    case 'assign':
                        $symbolVariable->setDynamicTypes('double');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                            $codePrinter->output($tempVariable->getName().' = '.$resolvedExpr->getCode().';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignDouble($symbolVariable, $tempVariable, $compilationContext);
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignDouble($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                        }
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->setDynamicTypes('bool');
                        if ('true' == $resolvedExpr->getCode()) {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignBool($symbolVariable, '1', $compilationContext);
                        } else {
                            if ('false' == $resolvedExpr->getCode()) {
                                $symbolVariable->initVariant($compilationContext);
                                $compilationContext->backend->assignBool($symbolVariable, '0', $compilationContext);
                            } else {
                                if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                                    $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                                    $codePrinter->output($tempVariable->getName().' = '.$resolvedExpr->getBooleanCode().';');
                                    $symbolVariable->initVariant($compilationContext);
                                    $compilationContext->backend->assignBool($symbolVariable, $tempVariable, $compilationContext);
                                } else {
                                    $symbolVariable->initVariant($compilationContext);
                                    $compilationContext->backend->assignBool($symbolVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                                }
                            }
                        }
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'string':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $symbolVariable->setDynamicTypes('string');
                        $compilationContext->backend->assignString($symbolVariable, add_slashes($resolvedExpr->getCode()), $compilationContext);
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output('zephir_concat_self_str(&'.$variable.', SL("'.$resolvedExpr->getCode().'"));');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'array':
                switch ($statement['operator']) {
                    case 'assign':
                        if ($variable != $resolvedExpr->getCode()) {
                            $symbolVariable->setMustInitNull(true);
                            $compilationContext->symbolTable->mustGrownStack(true);

                            /* Inherit the dynamic type data from the assigned value */
                            $symbolVariable->setDynamicTypes('array');
                            $symbolVariable->increaseVariantIfNull();
                            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                            $codePrinter->output('ZEPHIR_CPY_WRT('.$symbol.', '.$compilationContext->backend->resolveValue($resolvedExpr, $compilationContext).');');
                        }
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $compilationContext->backend->assignLong($symbolVariable, $itemVariable, $compilationContext);
                                break;

                            case 'add-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
                                $codePrinter->output($tempVariable->getName().' = zephir_get_numberval('.$compilationContext->backend->getVariableCode($symbolVariable).') + '.$itemVariable->getName().';');
                                $compilationContext->backend->assignLong($symbolVariable, $tempVariable, $compilationContext);
                                break;

                            case 'sub-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
                                $codePrinter->output($tempVariable->getName().' = zephir_get_numberval('.$compilationContext->backend->getVariableCode($symbolVariable).');');
                                $compilationContext->backend->assignLong($symbolVariable, $tempVariable->getName().' - '.$itemVariable->getName(), $compilationContext);
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('double');
                                $compilationContext->backend->assignDouble($symbolVariable, $itemVariable, $compilationContext);
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('bool');
                                $compilationContext->backend->assignBool($symbolVariable, $itemVariable, $compilationContext);
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'array':
                        switch ($statement['operator']) {
                            case 'assign':
                                if ($variable != $resolvedExpr->getCode()) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes('array');
                                    $symbolVariable->increaseVariantIfNull();
                                    $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                                    $codePrinter->output('ZEPHIR_CPY_WRT('.$symbol.', '.$compilationContext->backend->resolveValue($resolvedExpr, $compilationContext).');');
                                }
                                break;

                            default:
                                throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                        }
                        break;

                    case 'variable':
                        switch ($statement['operator']) {
                            case 'assign':
                                if ($itemVariable->getName() != $variable) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes(array_keys($itemVariable->getDynamicTypes()));

                                    $symbolVariable->setClassTypes($itemVariable->getClassTypes());
                                    $symbolVariable->increaseVariantIfNull();

                                    $compilationContext->backend->copyOnWrite($symbolVariable, $itemVariable, $compilationContext);
                                    if ($itemVariable->isTemporal()) {
                                        $itemVariable->setIdle(true);
                                    }
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf($symbolVariable, $itemVariable, $compilationContext);
                                break;

                            case 'add-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('ZEPHIR_ADD_ASSIGN('.$compilationContext->backend->getVariableCode($symbolVariable).', '.$compilationContext->backend->getVariableCode($itemVariable).');');
                                break;

                            case 'sub-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('ZEPHIR_SUB_ASSIGN('.$compilationContext->backend->getVariableCode($symbolVariable).', '.$compilationContext->backend->getVariableCode($itemVariable).');');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {
                            case 'assign':
                                if ($itemVariable->getName() != $variable) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes(array_keys($itemVariable->getDynamicTypes()));
                                    $symbolVariable->setClassTypes($itemVariable->getClassTypes());
                                    $symbolVariable->increaseVariantIfNull();

                                    $compilationContext->backend->copyOnWrite($symbolVariable, $itemVariable, $compilationContext);
                                    if ($itemVariable->isTemporal()) {
                                        $itemVariable->setIdle(true);
                                    }
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf($symbolVariable, $itemVariable, $compilationContext);
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: '.$itemVariable->getType(), $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException('Unknown type: '.$resolvedExpr->getType(), $resolvedExpr->getOriginal());
        }
    }
}
