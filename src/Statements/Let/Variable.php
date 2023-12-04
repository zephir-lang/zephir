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

namespace Zephir\Statements\Let;

use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;
use Zephir\Name;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function array_keys;

/**
 * Zephir\Statements\Let\Variable.
 *
 * Assign a value to a variable.
 */
class Variable
{
    use VariablesTrait;

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
    ): void {
        $this->checkVariableReadOnly($variable, $symbolVariable, $statement);

        $codePrinter = $compilationContext->codePrinter;

        /*
         * Only initialize variables if it's direct assignment
         */
        if ('assign' == $statement['operator']) {
            $symbolVariable->setIsInitialized(true, $compilationContext);
        } else {
            $this->checkVariableInitialized($variable, $symbolVariable, $statement);
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
                $this->doNumericAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $variable,
                    $statement,
                    $compilationContext
                );
                break;

            case 'double':
                $this->doDoubleAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $variable,
                    $statement,
                    $compilationContext
                );
                break;

            case 'array':
                $this->doArrayAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $symbolVariable,
                    $variable,
                    $statement,
                    $compilationContext
                );
                break;

            case 'string':
                $this->doStringAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $symbolVariable,
                    $variable,
                    $statement,
                    $compilationContext
                );
                break;

            case 'bool':
                $this->doBoolAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $variable,
                    $statement,
                    $compilationContext
                );
                break;

            case 'variable':
            case 'mixed':
                $this->doVariableAssignment(
                    $codePrinter,
                    $resolvedExpr,
                    $symbolVariable,
                    $variable,
                    $statement,
                    $compilationContext,
                    $readDetector
                );
                break;

            default:
                throw new CompilerException('Unknown type: ' . $type, $statement);
        }
    }

    /**
     * Performs array assignment.
     *
     * @param Printer            $codePrinter
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
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ): void {
        switch ($resolvedExpr->getType()) {
            case 'variable':
            case 'array':
                $this->doArrayAssignmentProcess(
                    $statement,
                    $resolvedExpr,
                    $variable,
                    $symbolVariable,
                    $compilationContext,
                    $codePrinter
                );
                break;

            default:
                throw new CompilerException(
                    "Cannot '" . $statement['operator'] . "' " . $resolvedExpr->getType() . ' for array type',
                    $resolvedExpr->getOriginal()
                );
        }
    }

    /**
     * @param array              $statement
     * @param CompiledExpression $resolvedExpr
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompilationContext $compilationContext
     * @param Printer            $codePrinter
     *
     * @return void
     */
    private function doArrayAssignmentProcess(
        array $statement,
        CompiledExpression $resolvedExpr,
        string $variable,
        ZephirVariable $symbolVariable,
        CompilationContext $compilationContext,
        Printer $codePrinter
    ): void {
        switch ($statement['operator']) {
            case 'assign':
                if ($variable != $resolvedExpr->getCode()) {
                    $symbolVariable->setMustInitNull(true);
                    $compilationContext->symbolTable->mustGrownStack(true);

                    /* Inherit the dynamic type data from the assigned value */
                    $symbolVariable->setDynamicTypes('array');
                    $symbolVariable->increaseVariantIfNull();
                    $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                    $codePrinter->output(
                        'ZEPHIR_CPY_WRT(' . $symbol . ', ' . $compilationContext->backend->resolveValue(
                            $resolvedExpr,
                            $compilationContext
                        ) . ');'
                    );
                }
                break;

            default:
                throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
        }
    }

    /**
     * Performs boolean assignment.
     *
     * @param Printer            $codePrinter
     * @param CompiledExpression $resolvedExpr
     * @param string             $variable
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doBoolAssignment(
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ): void {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = 0;');
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
                        $codePrinter->output($variable . ' = ((' . $resolvedExpr->getCode() . ') ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'double':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ((' . $resolvedExpr->getCode() . ' != 0.0) ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ((\'' . $resolvedExpr->getCode() . '\') ? 1 : 0);');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = ((' . $itemVariable->getName() . ') ? 1 : 0);');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output(
                                    $variable . ' = ((' . $itemVariable->getName() . ' != 0.0) ? 1 : 0);'
                                );
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                    case 'mixed':
                    case 'string':
                    case 'array':
                        switch ($statement['operator']) {
                            case 'assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    $variable . ' = zephir_is_true(' . $compilationContext->backend->getVariableCode(
                                        $itemVariable
                                    ) . ');'
                                );
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Cannot assign variable: ' . $itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type: ' . $resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs double assignment.
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doDoubleAssignment(
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ): void {
        switch ($resolvedExpr->getType()) {
            case 'null':
                $this->doNumberAssignmentNull(
                    $statement,
                    $codePrinter,
                    $variable,
                    $resolvedExpr,
                    '0.0'
                );
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $this->doNumericAssignmentLong(
                    $statement,
                    $codePrinter,
                    $variable,
                    $resolvedExpr,
                    '(double)'
                );
                break;

            case 'double':
                $this->doNumericAssignmentLong(
                    $statement,
                    $codePrinter,
                    $variable,
                    $resolvedExpr
                );
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'bool':
                        $this->doNumericAssignmentVar(
                            $statement,
                            $codePrinter,
                            $variable,
                            $itemVariable,
                            '(double)'
                        );
                        break;

                    case 'double':
                        $this->doNumericAssignmentVar(
                            $statement,
                            $codePrinter,
                            $variable,
                            $itemVariable
                        );
                        break;

                    case 'variable':
                    case 'mixed':
                        $compilationContext->headersManager->add('kernel/operators');
                        $this->processDoNumericAssignmentMixed(
                            $compilationContext,
                            $itemVariable,
                            $statement,
                            $codePrinter,
                            $variable,
                            $itemVariable
                        );
                        break;

                    default:
                        throw new CompilerException('Unknown type: ' . $itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException('Unknown type ' . $resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs numeric assignment.
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doNumericAssignment(
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ): void {
        switch ($resolvedExpr->getType()) {
            case 'null':
                $this->doNumberAssignmentNull(
                    $statement,
                    $codePrinter,
                    $variable,
                    $resolvedExpr,
                    '0'
                );
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'div-assign':
                        $codePrinter->output($variable . ' /= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'mod-assign':
                        $codePrinter->output($variable . ' %= ' . $resolvedExpr->getCode() . ';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'double':
                $this->doNumericAssignmentLong(
                    $statement,
                    $codePrinter,
                    $variable,
                    $resolvedExpr,
                    '(long)'
                );
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
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
                                $codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
                                break;

                            case 'add-assign':
                                $codePrinter->output($variable . ' += ' . $itemVariable->getName() . ';');
                                break;

                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= ' . $itemVariable->getName() . ';');
                                break;

                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= ' . $itemVariable->getName() . ';');
                                break;

                            case 'div-assign':
                                $codePrinter->output($variable . ' /= ' . $itemVariable->getName() . ';');
                                break;

                            case 'mod-assign':
                                $codePrinter->output($variable . ' %= ' . $itemVariable->getName() . ';');
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'double':
                        $this->doNumericAssignmentVar(
                            $statement,
                            $codePrinter,
                            $variable,
                            $itemVariable,
                            '(long)'
                        );
                        break;

                    case 'variable':
                    case 'mixed':
                        $compilationContext->headersManager->add('kernel/operators');
                        $exprVariable = $compilationContext->symbolTable->getVariableForWrite(
                            $resolvedExpr->resolve(null, $compilationContext),
                            $compilationContext
                        );
                        $this->processDoNumericAssignmentMixed(
                            $compilationContext,
                            $exprVariable,
                            $statement,
                            $codePrinter,
                            $variable,
                            $itemVariable
                        );
                        break;

                    default:
                        throw new CompilerException('Unknown type: ' . $itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException(
                    "Value type '" . $resolvedExpr->getType() . "' cannot be assigned to variable: int",
                    $statement
                );
        }
    }

    /**
     * Performs string assignment.
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     */
    private function doStringAssignment(
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext
    ): void {
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
                        $compilationContext->backend->assignString(
                            $symbolVariable,
                            $resolvedExpr->getCode(),
                            $compilationContext
                        );
                        break;
                    case 'concat-assign':
                        $codePrinter->output(
                            'zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode(
                            ) . '", sizeof("' . $resolvedExpr->getCode() . '") - 1);'
                        );
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
                            $compilationContext->backend->assignString(
                                $symbolVariable,
                                $resolvedExpr->getCode(),
                                $compilationContext
                            );
                        } else {
                            $compilationContext->backend->assignString($symbolVariable, null, $compilationContext);
                        }
                        break;
                    case 'concat-assign':
                        $codePrinter->output(
                            'zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode(
                            ) . '", sizeof("' . $resolvedExpr->getCode() . '") - 1);'
                        );
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
                            $compilationContext->backend->assignString(
                                $symbolVariable,
                                $resolvedExpr->getCode(),
                                $compilationContext
                            );
                        } else {
                            $compilationContext->backend->assignString($symbolVariable, null, $compilationContext);
                        }
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output(
                            'zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode(
                            ) . '", sizeof("' . $resolvedExpr->getCode() . '") - 1);'
                        );
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
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
                                $codePrinter->output(
                                    'Z_STRLEN_P(' . $variable . ') = zephir_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%ld", ' . $itemVariable->getName(
                                    ) . ');'
                                );
                                $codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    'zephir_concat_self_long(&' . $variable . ', ' . $itemVariable->getName() . ');'
                                );
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
                                $codePrinter->output(
                                    'Z_STRLEN_P(' . $variable . ') = zephir_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%c", ' . $itemVariable->getName(
                                    ) . ');'
                                );
                                $codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    'zephir_concat_self_char(&' . $variable . ', ' . $itemVariable->getName() . ');'
                                );
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
                                    $compilationContext->backend->copyOnWrite(
                                        $symbolVariable,
                                        $itemVariable,
                                        $compilationContext
                                    );
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'variable':
                    case 'mixed':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->setMustInitNull(true);
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    'zephir_get_strval(' . $compilationContext->backend->getVariableCode(
                                        $symbolVariable
                                    ) . ', ' . $compilationContext->backend->getVariableCode($itemVariable) . ');'
                                );
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: ' . $itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type ' . $resolvedExpr->getType(), $statement);
        }
    }

    /**
     * Performs variable assignment.
     *
     * @param Printer            $codePrinter
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
        Printer $codePrinter,
        CompiledExpression $resolvedExpr,
        ZephirVariable $symbolVariable,
        string $variable,
        array $statement,
        CompilationContext $compilationContext,
        ReadDetector $readDetector
    ): void {
        switch ($resolvedExpr->getType()) {
            case 'null':
                if ($statement['operator'] == 'assign') {
                    $symbolVariable->initVariant($compilationContext);
                    $symbolVariable->setDynamicTypes('null');

                    $compilationContext->backend->assignNull($symbolVariable, $compilationContext);
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

                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignLong(
                            $tempVariable,
                            $resolvedExpr->getCode(),
                            $compilationContext
                        );

                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output(
                            $functionName . '(' . $symbol . ', ' . $compilationContext->backend->getVariableCode(
                                $tempVariable
                            ) . ');'
                        );
                        break;

                    case 'assign':
                        $symbolVariable->setDynamicTypes('long');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                'int',
                                $compilationContext
                            );
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong(
                                $symbolVariable,
                                $tempVariable,
                                $compilationContext
                            );
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong(
                                $symbolVariable,
                                $resolvedExpr->getCode(),
                                $compilationContext
                            );
                        }
                        break;

                    case 'div-assign':
                        $this->processDoVariableAssignmentAssign(
                            $symbolVariable,
                            $readDetector,
                            $variable,
                            $resolvedExpr,
                            $compilationContext,
                            $codePrinter
                        );
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
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                'char',
                                $compilationContext
                            );
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong(
                                $symbolVariable,
                                $tempVariable,
                                $compilationContext
                            );
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $compilationContext->backend->assignLong(
                                $symbolVariable,
                                '\'' . $resolvedExpr->getCode() . '\'',
                                $compilationContext
                            );
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

                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $tempVariable->setDynamicTypes('double');
                        $compilationContext->backend->assignDouble(
                            $tempVariable,
                            $resolvedExpr->getCode(),
                            $compilationContext
                        );

                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output(
                            $functionName . '(' . $compilationContext->backend->getVariableCode(
                                $symbolVariable
                            ) . ', ' . $compilationContext->backend->getVariableCode($tempVariable) . ');'
                        );
                        break;

                    case 'assign':
                        $this->processDoVariableAssignmentAssign(
                            $symbolVariable,
                            $readDetector,
                            $variable,
                            $resolvedExpr,
                            $compilationContext,
                            $codePrinter
                        );
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
                                    $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                        'double',
                                        $compilationContext
                                    );
                                    $codePrinter->output(
                                        $tempVariable->getName() . ' = ' . $resolvedExpr->getBooleanCode() . ';'
                                    );
                                    $symbolVariable->initVariant($compilationContext);
                                    $compilationContext->backend->assignBool(
                                        $symbolVariable,
                                        $tempVariable,
                                        $compilationContext
                                    );
                                } else {
                                    $symbolVariable->initVariant($compilationContext);
                                    $compilationContext->backend->assignBool(
                                        $symbolVariable,
                                        $resolvedExpr->getBooleanCode(),
                                        $compilationContext
                                    );
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
                        $compilationContext->backend->assignString(
                            $symbolVariable,
                            Name::addSlashes($resolvedExpr->getCode()),
                            $compilationContext
                        );
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output(
                            'zephir_concat_self_str(&' . $variable . ', SL("' . $resolvedExpr->getCode() . '"));'
                        );
                        break;

                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr, $resolvedExpr->getOriginal());
                }
                break;

            case 'array':
                $this->doArrayAssignmentProcess(
                    $statement,
                    $resolvedExpr,
                    $variable,
                    $symbolVariable,
                    $compilationContext,
                    $codePrinter
                );
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $resolvedExpr->getOriginal()
                );
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
                                $compilationContext->backend->assignLong(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;

                            case 'add-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                    $itemVariable->getType(),
                                    $compilationContext
                                );
                                $codePrinter->output(
                                    $tempVariable->getName(
                                    ) . ' = zephir_get_numberval(' . $compilationContext->backend->getVariableCode(
                                        $symbolVariable
                                    ) . ') + ' . $itemVariable->getName() . ';'
                                );
                                $compilationContext->backend->assignLong(
                                    $symbolVariable,
                                    $tempVariable,
                                    $compilationContext
                                );
                                break;

                            case 'sub-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                    $itemVariable->getType(),
                                    $compilationContext
                                );
                                $codePrinter->output(
                                    $tempVariable->getName(
                                    ) . ' = zephir_get_numberval(' . $compilationContext->backend->getVariableCode(
                                        $symbolVariable
                                    ) . ');'
                                );
                                $compilationContext->backend->assignLong(
                                    $symbolVariable,
                                    $tempVariable->getName() . ' - ' . $itemVariable->getName(),
                                    $compilationContext
                                );
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
                                $compilationContext->backend->assignDouble(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
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
                                $compilationContext->backend->assignBool(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;
                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'array':
                        $this->doArrayAssignmentProcess(
                            $statement,
                            $resolvedExpr,
                            $variable,
                            $symbolVariable,
                            $compilationContext,
                            $codePrinter
                        );
                        break;

                    case 'variable':
                        switch ($statement['operator']) {
                            case 'assign':
                                $this->processDoVariableAssignmentVariableString(
                                    $itemVariable,
                                    $variable,
                                    $symbolVariable,
                                    $compilationContext
                                );
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;

                            case 'add-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    'ZEPHIR_ADD_ASSIGN(' . $compilationContext->backend->getVariableCode(
                                        $symbolVariable
                                    ) . ', ' . $compilationContext->backend->getVariableCode($itemVariable) . ');'
                                );
                                break;

                            case 'sub-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output(
                                    'ZEPHIR_SUB_ASSIGN(' . $compilationContext->backend->getVariableCode(
                                        $symbolVariable
                                    ) . ', ' . $compilationContext->backend->getVariableCode($itemVariable) . ');'
                                );
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {
                            case 'assign':
                                $this->processDoVariableAssignmentVariableString(
                                    $itemVariable,
                                    $variable,
                                    $symbolVariable,
                                    $compilationContext
                                );
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $compilationContext->backend->concatSelf(
                                    $symbolVariable,
                                    $itemVariable,
                                    $compilationContext
                                );
                                break;

                            default:
                                throw new IllegalOperationException($statement, $itemVariable);
                        }
                        break;

                    default:
                        throw new CompilerException(
                            'Unknown type: ' . $itemVariable->getType(),
                            $resolvedExpr->getOriginal()
                        );
                }
                break;

            default:
                throw new CompilerException('Unknown type: ' . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
        }
    }

    /**
     * @param array              $statement
     * @param Printer            $codePrinter
     * @param string             $variable
     * @param CompiledExpression $resolvedExpr
     * @param string             $value
     *
     * @return void
     */
    private function doNumberAssignmentNull(
        array $statement,
        Printer $codePrinter,
        string $variable,
        CompiledExpression $resolvedExpr,
        string $value
    ): void {
        switch ($statement['operator']) {
            case 'assign':
                $codePrinter->output($variable . ' = ' . $value . ';');
                break;
            case 'add-assign':
                $codePrinter->output($variable . ' += ' . $value . ';');
                break;
            case 'sub-assign':
                $codePrinter->output($variable . ' -= ' . $value . ';');
                break;
            case 'mul-assign':
                $codePrinter->output($variable . ' *= ' . $value . ';');
                break;
            default:
                throw new IllegalOperationException($statement, $resolvedExpr);
        }
    }

    /**
     * @param array               $statement
     * @param Printer             $codePrinter
     * @param string              $variable
     * @param ZephirVariable|bool $itemVariable
     * @param string              $cast
     *
     * @return void
     */
    private function doNumericAssignmentVar(
        array $statement,
        Printer $codePrinter,
        string $variable,
        ZephirVariable | bool $itemVariable,
        string $cast = ''
    ): void {
        switch ($statement['operator']) {
            case 'assign':
                $codePrinter->output(
                    $variable . ' = ' . $cast . ' ' . $itemVariable->getName() . ';'
                );
                break;
            case 'add-assign':
                $codePrinter->output(
                    $variable . ' += ' . $cast . ' ' . $itemVariable->getName() . ';'
                );
                break;
            case 'sub-assign':
                $codePrinter->output(
                    $variable . ' -= ' . $cast . ' ' . $itemVariable->getName() . ';'
                );
                break;
            case 'mul-assign':
                $codePrinter->output(
                    $variable . ' *= ' . $cast . ' ' . $itemVariable->getName() . ';'
                );
                break;
            default:
                throw new IllegalOperationException($statement, $itemVariable);
        }
    }

    /**
     * @param array              $statement
     * @param Printer            $codePrinter
     * @param string             $variable
     * @param CompiledExpression $resolvedExpr
     * @param string             $cast
     *
     * @return void
     */
    private function doNumericAssignmentLong(
        array $statement,
        Printer $codePrinter,
        string $variable,
        CompiledExpression $resolvedExpr,
        string $cast = ''
    ): void {
        switch ($statement['operator']) {
            case 'assign':
                $codePrinter->output(
                    $variable . ' = ' . $cast
                    . ' (' . $resolvedExpr->getCode() . ');'
                );
                break;

            case 'add-assign':
                $codePrinter->output(
                    $variable . ' += ' . $cast
                    . ' (' . $resolvedExpr->getCode() . ');'
                );
                break;

            case 'sub-assign':
                $codePrinter->output(
                    $variable . ' -= ' . $cast
                    . ' (' . $resolvedExpr->getCode() . ');'
                );
                break;

            case 'mul-assign':
                $codePrinter->output(
                    $variable . ' *= ' . $cast
                    . ' (' . $resolvedExpr->getCode() . ');'
                );
                break;

            default:
                throw new IllegalOperationException($statement, $resolvedExpr);
        }
    }

    /**
     * @param CompilationContext  $compilationContext
     * @param ZephirVariable|bool $exprVariable
     * @param array               $statement
     * @param Printer             $codePrinter
     * @param string              $variable
     * @param ZephirVariable|bool $itemVariable
     *
     * @return void
     */
    private function processDoNumericAssignmentMixed(
        CompilationContext $compilationContext,
        ZephirVariable | bool $exprVariable,
        array $statement,
        Printer $codePrinter,
        string $variable,
        ZephirVariable | bool $itemVariable
    ): void {
        $exprVariableCode = $compilationContext->backend->getVariableCode($exprVariable);
        switch ($statement['operator']) {
            case 'assign':
                $codePrinter->output($variable . ' = zephir_get_numberval(' . $exprVariableCode . ');');
                break;

            case 'add-assign':
                $codePrinter->output(
                    $variable . ' += zephir_get_numberval(' . $exprVariableCode . ');'
                );
                break;

            case 'sub-assign':
                $codePrinter->output(
                    $variable . ' -= zephir_get_numberval(' . $exprVariableCode . ');'
                );
                break;

            case 'mul-assign':
                $codePrinter->output(
                    $variable . ' *= zephir_get_numberval(' . $exprVariableCode . ');'
                );
                break;

            default:
                throw new IllegalOperationException($statement, $itemVariable);
        }
    }

    /**
     * @param ZephirVariable     $symbolVariable
     * @param ReadDetector       $readDetector
     * @param string             $variable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param Printer            $codePrinter
     *
     * @return void
     */
    private function processDoVariableAssignmentAssign(
        ZephirVariable $symbolVariable,
        ReadDetector $readDetector,
        string $variable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        Printer $codePrinter
    ): void {
        $symbolVariable->setDynamicTypes('double');
        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'double',
                $compilationContext
            );
            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
            $symbolVariable->initVariant($compilationContext);
            $compilationContext->backend->assignDouble(
                $symbolVariable,
                $tempVariable,
                $compilationContext
            );
        } else {
            $symbolVariable->initVariant($compilationContext);
            $compilationContext->backend->assignDouble(
                $symbolVariable,
                $resolvedExpr->getCode(),
                $compilationContext
            );
        }
    }

    /**
     * @param ZephirVariable|bool $itemVariable
     * @param string              $variable
     * @param ZephirVariable      $symbolVariable
     * @param CompilationContext  $compilationContext
     *
     * @return void
     */
    private function processDoVariableAssignmentVariableString(
        ZephirVariable | bool $itemVariable,
        string $variable,
        ZephirVariable $symbolVariable,
        CompilationContext $compilationContext
    ): void {
        if ($itemVariable->getName() != $variable) {
            $symbolVariable->setMustInitNull(true);
            $compilationContext->symbolTable->mustGrownStack(true);

            /* Inherit the dynamic type data from the assigned value */
            $symbolVariable->setDynamicTypes(array_keys($itemVariable->getDynamicTypes()));
            $symbolVariable->setClassTypes($itemVariable->getClassTypes());
            $symbolVariable->increaseVariantIfNull();

            $compilationContext->backend->copyOnWrite(
                $symbolVariable,
                $itemVariable,
                $compilationContext
            );
            if ($itemVariable->isTemporal()) {
                $itemVariable->setIdle(true);
            }
        }
    }
}
