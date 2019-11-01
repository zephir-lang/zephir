<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;

/**
 * Zephir\Call.
 *
 * Base class for common functionality in functions/calls
 */
class Call
{
    /**
     * Call expression.
     *
     * @var Expression
     */
    protected $expression;

    protected $mustInit;

    protected $symbolVariable;

    protected $isExpecting = false;

    protected $resolvedParams;

    protected $reflection;

    protected $resolvedTypes = [];

    protected $resolvedDynamicTypes = [];

    protected $temporalVariables = [];

    protected $mustCheckForCopy = [];

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call.
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedReturn(CompilationContext $compilationContext)
    {
        $expr = $this->expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed.
         */
        $mustInit = false;
        $symbolVariable = null;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (\is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            }
        }

        $this->mustInit = $mustInit;
        $this->symbolVariable = $symbolVariable;
        $this->isExpecting = $isExpecting;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call.
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedObservedReturn(CompilationContext $compilationContext)
    {
        $expr = $this->expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed.
         */
        $mustInit = false;
        $symbolVariable = null;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (\is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, $expression);
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, $expression);
            }
        }

        $this->mustInit = $mustInit;
        $this->symbolVariable = $symbolVariable;
        $this->isExpecting = $isExpecting;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call. If a temporal variable is used
     * as returned value only the body is freed between calls.
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedComplexLiteralReturn(CompilationContext $compilationContext)
    {
        $expr = $this->expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed.
         */
        $mustInit = false;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (\is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
            }
        }

        $this->mustInit = $mustInit;
        $this->symbolVariable = $symbolVariable;
        $this->isExpecting = $isExpecting;
    }

    /**
     * Check if an external expression is expecting the call return a value.
     *
     * @return bool
     */
    public function isExpectingReturn()
    {
        return $this->isExpecting;
    }

    /**
     * Returns if the symbol to be returned by the call must be initialized.
     *
     * @return bool
     */
    public function mustInitSymbolVariable()
    {
        return $this->mustInit;
    }

    /**
     * Returns the symbol variable that must be returned by the call.
     *
     * @param bool                    $useTemp
     * @param CompilationContext|null $compilationContext
     *
     * @return Variable
     */
    public function getSymbolVariable($useTemp = false, CompilationContext $compilationContext = null)
    {
        $symbolVariable = $this->symbolVariable;

        if ($useTemp && !\is_object($symbolVariable)) {
            return $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
        }

        return $symbolVariable;
    }

    /**
     * Resolves parameters.
     *
     * @param array              $parameters
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param bool               $readOnly
     *
     * @throws CompilerException
     *
     * @return array|CompiledExpression[]|null
     * @return array
     */
    public function getResolvedParamsAsExpr($parameters, CompilationContext $compilationContext, $expression, $readOnly = false)
    {
        if (!$this->resolvedParams) {
            $hasParametersByName = false;
            foreach ($parameters as $parameter) {
                if (isset($parameter['name'])) {
                    $hasParametersByName = true;
                    break;
                }
            }

            /*
             * All parameters must be passed by name
             */
            if ($hasParametersByName) {
                foreach ($parameters as $parameter) {
                    if (!isset($parameter['name'])) {
                        throw new CompilerException('All parameters must use named', $parameter);
                    }
                }
            }

            if ($hasParametersByName) {
                if ($this->reflection) {
                    $positionalParameters = [];
                    foreach ($this->reflection->getParameters() as $position => $reflectionParameter) {
                        if (\is_object($reflectionParameter)) {
                            $positionalParameters[$reflectionParameter->getName()] = $position;
                        } else {
                            $positionalParameters[$reflectionParameter['name']] = $position;
                        }
                    }
                    $orderedParameters = [];
                    foreach ($parameters as $parameter) {
                        if (isset($positionalParameters[$parameter['name']])) {
                            $orderedParameters[$positionalParameters[$parameter['name']]] = $parameter;
                        } else {
                            throw new CompilerException('Named parameter "'.$parameter['name'].'" is not a valid parameter name, available: '.implode(', ', array_keys($positionalParameters)), $parameter['parameter']);
                        }
                    }
                    $parameters_count = \count($parameters);
                    for ($i = 0; $i < $parameters_count; ++$i) {
                        if (!isset($orderedParameters[$i])) {
                            $orderedParameters[$i] = ['parameter' => ['type' => 'null']];
                        }
                    }
                    $parameters = $orderedParameters;
                }
            }

            $params = [];
            foreach ($parameters as $parameter) {
                if (\is_array($parameter['parameter'])) {
                    $paramExpr = new Expression($parameter['parameter']);

                    switch ($parameter['parameter']['type']) {
                        case 'property-access':
                        case 'array-access':
                        case 'static-property-access':
                            $paramExpr->setReadOnly(true);
                            break;

                        default:
                            $paramExpr->setReadOnly($readOnly);
                            break;
                    }

                    $params[] = $paramExpr->compile($compilationContext);
                    continue;
                }

                if ($parameter['parameter'] instanceof CompiledExpression) {
                    $params[] = $parameter['parameter'];
                    continue;
                }

                throw new CompilerException('Invalid expression ', $expression);
            }
            $this->resolvedParams = $params;
        }

        return $this->resolvedParams;
    }

    /**
     * Resolve parameters getting aware that the target function/method could retain or change
     * the parameters.
     *
     * @param array              $parameters
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param array              $calleeDefinition
     *
     * @throws CompilerException
     *
     * @return array
     */
    public function getResolvedParams($parameters, CompilationContext $compilationContext, array $expression, $calleeDefinition = null)
    {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression);

        /**
         * Static typed parameters in final/private methods are promotable to read only parameters
         * Recursive calls with static typed methods also also promotable.
         */
        $readOnlyParameters = [];
        if (\is_object($calleeDefinition)) {
            if ($calleeDefinition instanceof ClassMethod) {
                if ($calleeDefinition->isFinal() || $calleeDefinition->isPrivate() || $calleeDefinition->isInternal() || $compilationContext->currentMethod == $calleeDefinition) {
                    foreach ($calleeDefinition->getParameters() as $position => $parameter) {
                        if (isset($parameter['data-type'])) {
                            switch ($parameter['data-type']) {
                                case 'int':
                                case 'uint':
                                case 'double':
                                case 'long':
                                case 'char':
                                case 'uchar':
                                case 'boolean':
                                case 'bool':
                                    $readOnlyParameters[$position] = true;
                                    break;
                            }
                        }
                    }
                }
            }
        }

        $params = [];
        $types = [];
        $dynamicTypes = [];
        $mustCheck = [];
        foreach ($exprParams as $position => $compiledExpression) {
            $expression = $compiledExpression->getOriginal();
            switch ($compiledExpression->getType()) {
                case 'null':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignNull($parameterVariable, $compilationContext);
                    $this->temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignLong($parameterVariable, $compiledExpression->getCode(), $compilationContext);
                    $this->temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignDouble($parameterVariable, $compiledExpression->getCode(), $compilationContext);
                    $this->temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    break;

                case 'bool':
                    $value = $compiledExpression->getCode();
                    if ('true' == $value) {
                        $value = '1';
                    } elseif ('false' == $value) {
                        $value = '0';
                    }
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignBool($parameterVariable, $value, $compilationContext);

                    $this->temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'ulong':
                case 'string':
                case 'istring':
                    $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);

                    $compilationContext->backend->assignString(
                        $parameterVariable,
                        add_slashes($compiledExpression->getCode()),
                        $compilationContext,
                        true
                    );

                    $this->temporalVariables[] = $parameterVariable;
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'array':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'variable':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
                    switch ($parameterVariable->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        /* ulong must be stored in string */
                        case 'ulong':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                            $params[] = $compilationContext->backend->getVariableCode($parameterTempVariable);
                            $compilationContext->backend->assignLong($parameterTempVariable, $parameterVariable, $compilationContext);
                            $this->temporalVariables[] = $parameterTempVariable;
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                            $compilationContext->backend->assignDouble($parameterTempVariable, $parameterVariable, $compilationContext);
                            $params[] = $compilationContext->backend->getVariableCode($parameterTempVariable);
                            $this->temporalVariables[] = $parameterTempVariable;
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'bool':
                            $tempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                            $codePrinter->output('if ('.$parameterVariable->getName().') {');
                            $codePrinter->increaseLevel();
                            $compilationContext->backend->assignBool($tempVariable, '1', $compilationContext);
                            $codePrinter->decreaseLevel();
                            $codePrinter->output('} else {');
                            $codePrinter->increaseLevel();
                            $compilationContext->backend->assignBool($tempVariable, '0', $compilationContext);
                            $codePrinter->decreaseLevel();
                            $codePrinter->output('}');
                            $params[] = $compilationContext->backend->getVariableCode($tempVariable);
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'string':
                        case 'array':
                            $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'variable':
                            $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes = array_merge(
                                $dynamicTypes,
                                array_keys($parameterVariable->getDynamicTypes())
                            );
                            break;

                        default:
                            throw new CompilerException(
                                'Cannot use variable type: '.$parameterVariable->getType().' as parameter',
                                $expression
                            );
                    }
                    break;

                default:
                    throw new CompilerException(
                        'Cannot use value type: '.$compiledExpression->getType().' as parameter',
                        $expression
                    );
            }
        }

        $this->resolvedTypes = $types;
        $this->resolvedDynamicTypes = $dynamicTypes;
        $this->mustCheckForCopy = $mustCheck;

        return $params;
    }

    /**
     * Resolve parameters using zvals in the stack and without allocating memory for constants.
     *
     * @param array              $parameters
     * @param CompilationContext $compilationContext
     * @param array              $expression
     *
     * @throws CompilerException
     *
     * @return array
     */
    public function getReadOnlyResolvedParams($parameters, CompilationContext $compilationContext, array $expression)
    {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression, true);

        $params = [];
        $types = [];
        $dynamicTypes = [];

        foreach ($exprParams as $compiledExpression) {
            $expression = $compiledExpression->getOriginal();
            switch ($compiledExpression->getType()) {
                case 'null':
                    $params[] = $compilationContext->backend->resolveValue('null', $compilationContext);
                    $types[] = 'null';
                    $dynamicTypes[] = 'null';
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                    $compilationContext->backend->assignLong($parameterVariable, $compiledExpression->getCode(), $compilationContext);
                    $this->temporalVariables[] = $parameterVariable;
                    $params[] = '&'.$parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'char':
                case 'uchar':
                case 'ulong':
                case 'string':
                case 'istring':
                    $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );

                    $compilationContext->backend->assignString(
                        $parameterVariable,
                        add_slashes($compiledExpression->getCode()),
                        $compilationContext,
                        true
                    );

                    $this->temporalVariables[] = $parameterVariable;
                    $params[] = '&'.$parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                    $codePrinter->output('ZVAL_DOUBLE(&'.$parameterVariable->getName().', '.$compiledExpression->getCode().');');
                    $this->temporalVariables[] = $parameterVariable;
                    $params[] = '&'.$parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'bool':
                    if ('true' == $compiledExpression->getCode()) {
                        $params[] = $compilationContext->backend->resolveValue('true', $compilationContext);
                    } else {
                        if ('false' == $compiledExpression->getCode()) {
                            $params[] = $compilationContext->backend->resolveValue('false', $compilationContext);
                        } else {
                            throw new CompilerException(
                                'Unable to resolve parameter using zval',
                                $expression
                            );
                        }
                    }
                    $types[] = 'bool';
                    $dynamicTypes[] = 'bool';
                    break;

                case 'array':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'variable':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
                    switch ($parameterVariable->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                            $codePrinter->output('ZVAL_LONG(&'.$parameterTempVariable->getName().', '.$compiledExpression->getCode().');');
                            $params[] = '&'.$parameterTempVariable->getName();
                            $types[] = $parameterTempVariable->getType();
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $this->temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'char':
                        case 'uchar':
                            $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                'variable',
                                $compilationContext,
                                $expression
                            );
                            $codePrinter->output(
                                sprintf(
                                    'ZVAL_STRINGL(&%s, &%s, 1);',
                                    $parameterVariable->getName(),
                                    $compiledExpression->getCode()
                                )
                            );

                            $this->temporalVariables[] = $parameterVariable;
                            $params[] = '&'.$parameterVariable->getName();
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                            $codePrinter->output('ZVAL_DOUBLE(&'.$parameterTempVariable->getName().', '.$compiledExpression->getCode().');');
                            $params[] = '&'.$parameterTempVariable->getName();
                            $types[] = $parameterTempVariable->getType();
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $this->temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'bool':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                            $compilationContext->backend->assignBool($parameterTempVariable, '('.$parameterVariable->getName().' ? 1 : 0)', $compilationContext);
                            $params[] = $compilationContext->backend->getVariableCode($parameterTempVariable);
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $types[] = $parameterTempVariable->getType();
                            break;

                        case 'string':
                        case 'variable':
                        case 'array':
                            $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                            $dynamicTypes[] = $parameterVariable->getType();
                            $types[] = $parameterVariable->getType();
                            break;

                        default:
                            throw new CompilerException(
                                'Cannot use variable type: '.$parameterVariable->getType().' as parameter',
                                $expression
                            );
                    }
                    break;

                default:
                    throw new CompilerException(
                        'Cannot use value type: '.$compiledExpression->getType().' as parameter',
                        $expression
                    );
            }
        }

        $this->resolvedTypes = $types;
        $this->resolvedDynamicTypes = $dynamicTypes;

        return $params;
    }

    /**
     * Add the last-call-status flag to the current symbol table.
     *
     * @param CompilationContext $compilationContext
     */
    public function addCallStatusFlag(CompilationContext $compilationContext)
    {
        if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_LAST_CALL_STATUS')) {
            $callStatus = new Variable('int', 'ZEPHIR_LAST_CALL_STATUS', $compilationContext->branchManager->getCurrentBranch());
            $callStatus->setIsInitialized(true, $compilationContext);
            $callStatus->increaseUses();
            $callStatus->setReadOnly(true);
            $compilationContext->symbolTable->addRawVariable($callStatus);
        }
    }

    /**
     * Checks the last call status or make a label jump to the next catch block.
     *
     * @param CompilationContext $compilationContext
     */
    public function addCallStatusOrJump(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/fcall');
        if ($compilationContext->insideTryCatch) {
            $compilationContext->codePrinter->output(
                'zephir_check_call_status_or_jump(try_end_'.$compilationContext->currentTryCatch.');'
            );

            return;
        }

        $compilationContext->codePrinter->output('zephir_check_call_status();');
    }

    /**
     * Checks if temporary parameters must be copied or not.
     *
     * @param CompilationContext $compilationContext
     */
    public function checkTempParameters(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/fcall');
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $compilationContext->codePrinter->output('zephir_check_temp_parameter('.$checkVariable.');');
        }
    }

    /**
     * Return resolved parameter types.
     *
     * @return array
     */
    public function getResolvedTypes()
    {
        return $this->resolvedTypes;
    }

    /**
     * Return resolved parameter dynamic types.
     *
     * @return array
     */
    public function getResolvedDynamicTypes()
    {
        return $this->resolvedDynamicTypes;
    }

    /**
     * Returns the temporal variables generated during the parameter resolving.
     *
     * @return Variable[]
     */
    public function getTemporalVariables()
    {
        return $this->temporalVariables;
    }

    /**
     * Parameters to check if they must be copied.
     *
     * @return array
     */
    public function getMustCheckForCopyVariables()
    {
        return $this->mustCheckForCopy;
    }
}
