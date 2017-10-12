<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Compiler\CompilerException;
use Zephir\Detectors\ReadDetector;

/**
 * Call
 *
 * Base class for common functionality in functions/calls
 */
class Call
{
    /**
     * Call expression
     * @var Expression
     */
    protected $_expression;

    protected $_mustInit;

    protected $_symbolVariable;

    protected $_isExpecting = false;

    protected $_resolvedParams;

    protected $_reflection;

    protected $_resolvedTypes = [];

    protected $_resolvedDynamicTypes = [];

    protected $_temporalVariables = [];

    protected $_mustCheckForCopy = [];

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedReturn(CompilationContext $compilationContext)
    {
        $expr = $this->_expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed
         */
        $mustInit = false;
        $symbolVariable = null;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
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

        $this->_mustInit = $mustInit;
        $this->_symbolVariable = $symbolVariable;
        $this->_isExpecting = $isExpecting;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedObservedReturn(CompilationContext $compilationContext)
    {
        $expr = $this->_expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed
         */
        $mustInit = false;
        $symbolVariable = null;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
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

        $this->_mustInit = $mustInit;
        $this->_symbolVariable = $symbolVariable;
        $this->_isExpecting = $isExpecting;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call. If a temporal variable is used
     * as returned value only the body is freed between calls
     *
     * @param CompilationContext $compilationContext
     */
    public function processExpectedComplexLiteralReturn(CompilationContext $compilationContext)
    {
        $expr = $this->_expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed
         */
        $mustInit = false;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
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

        $this->_mustInit = $mustInit;
        $this->_symbolVariable = $symbolVariable;
        $this->_isExpecting = $isExpecting;
    }

    /**
     * Check if an external expression is expecting the call return a value
     *
     * @return boolean
     */
    public function isExpectingReturn()
    {
        return $this->_isExpecting;
    }

    /**
     * Returns if the symbol to be returned by the call must be initialized
     *
     * @return boolean
     */
    public function mustInitSymbolVariable()
    {
        return $this->_mustInit;
    }

    /**
     * Returns the symbol variable that must be returned by the call
     *
     * @param boolean $useTemp
     * @param CompilationContext $compilationContext
     * @return Variable
     */
    public function getSymbolVariable($useTemp = false, CompilationContext $compilationContext = null)
    {
        $symbolVariable = $this->_symbolVariable;
        if ($useTemp && !is_object($symbolVariable)) {
            return $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
        }
        return $symbolVariable;
    }

    /**
     * Resolves parameters
     *
     * @param array $parameters
     * @param CompilationContext $compilationContext
     * @param array $expression
     * @param boolean $readOnly
     * @return array|null|CompiledExpression[]
     *
     * @return array
     * @throws CompilerException
     */
    public function getResolvedParamsAsExpr($parameters, CompilationContext $compilationContext, $expression, $readOnly = false)
    {
        if (!$this->_resolvedParams) {
            $hasParametersByName = false;
            foreach ($parameters as $parameter) {
                if (isset($parameter['name'])) {
                    $hasParametersByName = true;
                    break;
                }
            }

            /**
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
                if ($this->_reflection) {
                    $positionalParameters = array();
                    foreach ($this->_reflection->getParameters() as $position => $reflectionParameter) {
                        if (is_object($reflectionParameter)) {
                            $positionalParameters[$reflectionParameter->getName()] = $position;
                        } else {
                            $positionalParameters[$reflectionParameter['name']] = $position;
                        }
                    }
                    $orderedParameters = array();
                    foreach ($parameters as $parameter) {
                        if (isset($positionalParameters[$parameter['name']])) {
                            $orderedParameters[$positionalParameters[$parameter['name']]] = $parameter;
                        } else {
                            throw new CompilerException('Named parameter "' . $parameter['name'] . '" is not a valid parameter name, available: ' . join(', ', array_keys($positionalParameters)), $parameter['parameter']);
                        }
                    }
                    $parameters_count = count($parameters);
                    for ($i = 0; $i < $parameters_count; $i++) {
                        if (!isset($orderedParameters[$i])) {
                            $orderedParameters[$i] = array('parameter' => array('type' => 'null'));
                        }
                    }
                    $parameters = $orderedParameters;
                }
            }

            $params = array();
            foreach ($parameters as $parameter) {
                if (is_array($parameter['parameter'])) {
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

                throw new CompilerException("Invalid expression ", $expression);
            }
            $this->_resolvedParams = $params;
        }

        return $this->_resolvedParams;
    }

    /**
     * Resolve parameters getting aware that the target function/method could retain or change
     * the parameters
     *
     * @param array $parameters
     * @param CompilationContext $compilationContext
     * @param array $expression
     * @param array $calleeDefinition
     * @return array
     *
     * @throws CompilerException
     */
    public function getResolvedParams($parameters, CompilationContext $compilationContext, array $expression, $calleeDefinition = null)
    {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression);

        /**
         * Static typed parameters in final/private methods are promotable to read only parameters
         * Recursive calls with static typed methods also also promotable
         */
        $isFinal = false;
        $readOnlyParameters = array();
        if (is_object($calleeDefinition)) {
            if ($calleeDefinition instanceof ClassMethod) {
                if ($calleeDefinition->isFinal() || $calleeDefinition->isPrivate() || $calleeDefinition->isInternal() || $compilationContext->currentMethod == $calleeDefinition) {
                    $isFinal = true;
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

        $params = array();
        $types = array();
        $dynamicTypes = array();
        $mustCheck = array();
        foreach ($exprParams as $position => $compiledExpression) {
            $expression = $compiledExpression->getOriginal();
            switch ($compiledExpression->getType()) {
                case 'null':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignNull($parameterVariable, $compilationContext);
                    $this->_temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignLong($parameterVariable, $compiledExpression->getCode(), $compilationContext);
                    $this->_temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignDouble($parameterVariable, $compiledExpression->getCode(), $compilationContext);
                    $this->_temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    break;

                case 'bool':
                    $value = $compiledExpression->getCode();
                    if ($value == 'true') {
                        $value = '1';
                    } else if ($value == 'false') {
                        $value = '0';
                    }
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                    $params[] = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignBool($parameterVariable, $value, $compilationContext);

                    $this->_temporalVariables[] = $parameterVariable;
                    $types[] = $compiledExpression->getType();
                    $dynamicTypes[] = $compiledExpression->getType();
                    break;

                case 'ulong':
                case 'string':
                case 'istring':
                    $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
                    $compilationContext->backend->assignString($parameterVariable, Utils::addSlashes($compiledExpression->getCode()), $compilationContext, true, 'ZEPHIR_TEMP_PARAM_COPY');
                    $this->_temporalVariables[] = $parameterVariable;
                    /* ZE3 copies strings */
                    if ($compilationContext->backend->getName() == 'ZendEngine2') {
                        $mustCheck[] = $parameterVariable->getName();
                    }
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
                            $this->_temporalVariables[] = $parameterTempVariable;
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                            $compilationContext->backend->assignDouble($parameterTempVariable, $parameterVariable, $compilationContext);
                            $params[] = $compilationContext->backend->getVariableCode($parameterTempVariable);
                            $this->_temporalVariables[] = $parameterTempVariable;
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'bool':
                            $tempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext);
                            $codePrinter->output('if (' . $parameterVariable->getName() . ') {');
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
                            $dynamicTypes[] = $parameterVariable->getDynamicTypes();
                            break;

                        default:
                            throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $expression);
                    }
                    break;

                default:
                    throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $expression);
            }
        }

        $this->_resolvedTypes = $types;
        $this->_resolvedDynamicTypes = $dynamicTypes;
        $this->_mustCheckForCopy = $mustCheck;
        return $params;
    }

    /**
     * Resolve parameters using zvals in the stack and without allocating memory for constants
     *
     * @param array $parameters
     * @param CompilationContext $compilationContext
     * @param array $expression
     *
     * @return array
     * @throws CompilerException
     * @throws Exception
     */
    public function getReadOnlyResolvedParams($parameters, CompilationContext $compilationContext, array $expression)
    {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression, true);

        $params = array();
        $types = array();
        $dynamicTypes = array();

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
                    $this->_temporalVariables[] = $parameterVariable;
                    $params[] = '&' . $parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'char':
                case 'uchar':
                    if ($compilationContext->backend->getName() == 'ZendEngine2') {
                        $parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
                    } else {
                        $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
                    }
                    $compilationContext->backend->assignString($parameterVariable, Utils::addSlashes($compiledExpression->getCode()), $compilationContext, true, false);

                    $this->_temporalVariables[] = $parameterVariable;
                    $params[] = '&' . $parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                    $codePrinter->output('ZVAL_DOUBLE(&' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
                    $this->_temporalVariables[] = $parameterVariable;
                    $params[] = '&' . $parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
                    break;

                case 'bool':
                    if ($compiledExpression->getCode() == 'true') {
                        $params[] = $compilationContext->backend->resolveValue('true', $compilationContext);
                    } else {
                        if ($compiledExpression->getCode() == 'false') {
                            $params[] = $compilationContext->backend->resolveValue('false', $compilationContext);
                        } else {
                            throw new Exception('?');
                        }
                    }
                    $types[] = 'bool';
                    $dynamicTypes[] = 'bool';
                    break;

                case 'ulong':
                case 'string':
                case 'istring':
                    if ($compilationContext->backend->getName() == 'ZendEngine2') {
                        $parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
                    } else {
                        $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
                    }
                    $compilationContext->backend->assignString($parameterVariable, Utils::addSlashes($compiledExpression->getCode()), $compilationContext, true, false);
                    $this->_temporalVariables[] = $parameterVariable;
                    $params[] = '&' . $parameterVariable->getName();
                    $types[] = $parameterVariable->getType();
                    $dynamicTypes[] = $parameterVariable->getType();
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
                            $codePrinter->output('ZVAL_LONG(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
                            $params[] = '&' . $parameterTempVariable->getName();
                            $types[] = $parameterTempVariable->getType();
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $this->_temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'char':
                        case 'uchar':
                            if ($compilationContext->backend->getName() == 'ZendEngine2') {
                                $parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
                                $codePrinter->output('ZVAL_STRINGL(&' . $parameterVariable->getName() . ', &' . $compiledExpression->getCode() . ', 1, 1);');
                            } else {
                                $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
                                $codePrinter->output('ZVAL_STRINGL(&' . $parameterVariable->getName() . ', &' . $compiledExpression->getCode() . ', 1);');
                            }
                            $this->_temporalVariables[] = $parameterVariable;
                            $params[] = '&' . $parameterVariable->getName();
                            $types[] = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                            $codePrinter->output('ZVAL_DOUBLE(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
                            $params[] = '&' . $parameterTempVariable->getName();
                            $types[] = $parameterTempVariable->getType();
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $this->_temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'bool':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable('variable', $compilationContext, true);
                            $compilationContext->backend->assignBool($parameterTempVariable, '(' . $parameterVariable->getName() . ' ? 1 : 0)', $compilationContext);
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
                            throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $expression);
                    }
                    break;

                default:
                    throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $expression);
            }
        }

        $this->_resolvedTypes = $types;
        $this->_resolvedDynamicTypes = $dynamicTypes;

        return $params;
    }

    /**
     * Add the last-call-status flag to the current symbol table
     *
     * @param CompilationContext $compilationContext
     */
    public function addCallStatusFlag(CompilationContext $compilationContext)
    {
        if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_LAST_CALL_STATUS')) {
            $callStatus = new Variable('int', 'ZEPHIR_LAST_CALL_STATUS', $compilationContext->currentBranch);
            $callStatus->setIsInitialized(true, $compilationContext);
            $callStatus->increaseUses();
            $callStatus->setReadOnly(true);
            $compilationContext->symbolTable->addRawVariable($callStatus);
        }
    }

    /**
     * Checks the last call status or make a label jump to the next catch block
     *
     * @param CompilationContext $compilationContext
     */
    public function addCallStatusOrJump(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/fcall');
        if ($compilationContext->insideTryCatch) {
            $compilationContext->codePrinter->output(
                'zephir_check_call_status_or_jump(try_end_' . $compilationContext->currentTryCatch . ');'
            );
            return;
        }

        $compilationContext->codePrinter->output('zephir_check_call_status();');
    }

    /**
     * Checks if temporary parameters must be copied or not
     *
     * @param CompilationContext $compilationContext
     */
    public function checkTempParameters(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/fcall');
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $compilationContext->codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }
    }

    /**
     * Return resolved parameter types
     *
     * @return array
     */
    public function getResolvedTypes()
    {
        return $this->_resolvedTypes;
    }

    /**
     * Return resolved parameter dynamic types
     *
     * @return array
     */
    public function getResolvedDynamicTypes()
    {
        return $this->_resolvedDynamicTypes;
    }

    /**
     * Returns the temporal variables generated during the parameter resolving
     *
     * @return Variable[]
     */
    public function getTemporalVariables()
    {
        return $this->_temporalVariables;
    }

    /**
     * Parameters to check if they must be copied
     *
     * @return array
     */
    public function getMustCheckForCopyVariables()
    {
        return $this->_mustCheckForCopy;
    }
}
