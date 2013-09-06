<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * Call
 *
 * Base class for common functionality in functions/calls
 */
class Call
{

	/**
	 * Call expression
	 */
	protected $_expression;

	protected $_mustInit;

	protected $_symbolVariable;

	protected $_isExpecting = false;

	protected $_resolvedParams;

	protected $_temporalVariables = array();

	/**
	 * Processes the symbol variable that will be used to return
	 * the result of the symbol call
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function processExpectedReturn($compilationContext)
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
				$readDetector = new ReadDetector($expression);
				if ($readDetector->detect($symbolVariable->getName(), $expression)) {
					$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
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
	 * the result of the symbol call. If a temporal variable is used
	 * as returned value only the body is freed between calls
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function processExpectedComplexLiteralReturn($compilationContext)
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
				$readDetector = new ReadDetector($expression);
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
	 * @return \Variable
	 */
	public function getSymbolVariable()
	{
		return $this->_symbolVariable;
	}

	/**
	 * Resolves paramameters
	 *
	 * @param array $parameters
	 * @param CompilationContext $compilationContext
	 * @param array $expression
	 * @return array
	 */
	public function getResolvedParamsAsExpr($parameters, $compilationContext, $expression)
	{
		if (!$this->_resolvedParams) {
			$params = array();
			foreach ($parameters as $parameter) {
				$paramExpr = new Expression($parameter);
				$params[] = $paramExpr->compile($compilationContext);
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
	 * @return array
	 */
	public function getResolvedParams($parameters, $compilationContext, $expression)
	{

		$codePrinter = $compilationContext->codePrinter;

		$exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression);

		$params = array();
		foreach ($exprParams as $compiledExpression) {
			$expression = $compiledExpression->getOriginal();
			switch ($compiledExpression->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_LONG(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = $parameterVariable->getName();
					break;
				case 'double':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_DOUBLE(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = $parameterVariable->getName();
					break;
				case 'bool':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_BOOL(' . $parameterVariable->getName() . ', ' . $compiledExpression->getBooleanCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = $parameterVariable->getName();
					break;
				case 'string':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_STRING(' . $parameterVariable->getName() . ', "' . $compiledExpression->getCode() . '", 1);');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = $parameterVariable->getName();
					break;
				case 'variable':
					$parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
					switch ($parameterVariable->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'ulong':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_LONG(' . $parameterTempVariable->getName() . ', ' . $parameterVariable->getName() . ');');
							$params[] = $parameterTempVariable->getName();
							$this->_temporalVariables[] = $parameterTempVariable;
							break;
						case 'bool':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_BOOL(' . $parameterTempVariable->getName() . ', ' . $parameterVariable->getName() . ');');
							$params[] = $parameterTempVariable->getName();
							$this->_temporalVariables[] = $parameterTempVariable;
							break;
						case 'string':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_STRINGL(' . $parameterTempVariable->getName() . ', ' . $parameterVariable->getName() . '->str, ' . $parameterVariable->getName() . '->len, 1);');
							$params[] = $parameterTempVariable->getName();
							$this->_temporalVariables[] = $parameterTempVariable;
							break;
						case 'variable':
							$params[] = $parameterVariable->getName();
							break;
						default:
							throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $expression);
					}
					break;
				default:
					throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $expression);
			}
		}

		return $params;
	}

	/**
	 * Resolve parameters using zvals in the stack and without allocating memory for constants
	 *
	 * @param array $parameters
	 * @param CompilationContext $compilationContext
	 * @param array $expression
	 * @return array
	 */
	public function getReadOnlyResolvedParams($parameters, $compilationContext, $expression)
	{

		$codePrinter = $compilationContext->codePrinter;

		$exprParams = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression);

		$params = array();
		foreach ($exprParams as $compiledExpression) {
			$expression = $compiledExpression->getOriginal();
			switch ($compiledExpression->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
					$parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_LONG(&' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = '&' . $parameterVariable->getName();
					break;
				case 'double':
					$parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_DOUBLE(&' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = '&' . $parameterVariable->getName();
					break;
				case 'bool':
					$parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_BOOL(&' . $parameterVariable->getName() . ', ' . $compiledExpression->getBooleanCode() . ');');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = '&' . $parameterVariable->getName();
					break;
				case 'string':
					$parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_STRING(&' . $parameterVariable->getName() . ', "' . $compiledExpression->getCode() . '", 0);');
					$this->_temporalVariables[] = $parameterVariable;
					$params[] = '&' . $parameterVariable->getName();
					break;
				case 'variable':
					$parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
					switch ($parameterVariable->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'ulong':
							$parameterTempVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_LONG(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
							$params[] = '&' . $parameterTempVariable->getName();
							$this->_temporalVariables[] = $parameterTempVariable;
							break;
						case 'string':
							$parameterTempVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_STRINGL(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . '->str, ' . $compiledExpression->getCode() . '->len, 0);');
							$params[] = '&' . $parameterTempVariable->getName();
							$this->_temporalVariables[] = $parameterTempVariable;
							break;
						case 'variable':
							$params[] = $parameterVariable->getName();
							break;
						default:
							throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $expression);
					}
					break;
				default:
					throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $expression);
			}
		}

		return $params;
	}

	/**
	 * Returns the temporal variables generated during the parameter resolving
	 *
	 * @return \Variable[]
	 */
	public function getTemporalVariables()
	{
		return $this->_temporalVariables;
	}

}