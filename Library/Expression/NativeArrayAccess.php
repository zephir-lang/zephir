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
 * NativeArrayAccess
 *
 * Resolves expressions that read array indexes
 */
class NativeArrayAccess
{
	protected $_expecting = true;

	protected $_readOnly = false;

	protected $_expectingVariable;

	/**
	 * Sets if the variable must be resolved into a direct variable symbol
	 * create a temporary value or ignore the return value
	 *
	 * @param boolean $expecting
	 * @param Variable $expectingVariable
	 */
	public function setExpectReturn($expecting, Variable $expectingVariable=null)
	{
		$this->_expecting = $expecting;
		$this->_expectingVariable = $expectingVariable;
	}

	/**
	 * Sets if the result of the evaluated expression is read only
	 *
	 * @param boolean $readOnly
	 */
	public function setReadOnly($readOnly)
	{
		$this->_readOnly = $readOnly;
	}

	/**
	 * Compiles foo[x] = {expr}
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$arrayAccess = $expression;

		$expr = new Expression($arrayAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $compilationContext, $expression);
				switch ($variableVariable->getType()) {
					case 'variable':
						break;
					default:
						throw new CompiledException("Variable type: " . $variableVariable->getType() . " cannot be used as array", $arrayAccess['left']);
				}
				break;
			default:
				throw new CompiledException("Cannot use expression: " . $exprVariable->getType() . " as an array", $arrayAccess['left']);
		}

		/**
		 * Variables that contain dynamic type data can be validated to be arrays
		 */
		$dynamicType = $variableVariable->getDynamicType();
		if ($dynamicType != 'undefined' && $dynamicType != 'array') {
			$compilationContext->logger->warning('Possible attempt to access index on a non-array dynamic variable', 'non-array-update', $expression);
		}

		/**
		 * Resolves the symbol that expects the value
		 */
		if ($this->_readOnly) {
			if ($this->_expecting) {
				if ($this->_expectingVariable) {
					$symbolVariable = $this->_expectingVariable;
					$symbolVariable->observeVariant($compilationContext);
					$this->_readOnly = false;
				} else {
					$symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, $expression);
				}
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, $expression);
			}
		} else {
			if ($this->_expecting) {
				if ($this->_expectingVariable) {
					$symbolVariable = $this->_expectingVariable;
					$symbolVariable->observeVariant($compilationContext);
				} else {
					$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
				}
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
			}
		}

		/**
		 * Variable that receives property accesses must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign array index", $expression);
		}

		/**
		 * At this point, we don't know the type fetched from the index
		 */
		$symbolVariable->setDynamicType('undefined');

		if ($this->_readOnly) {
			$flags = 'PH_NOISY | PH_READONLY';
		} else {
			$flags = 'PH_NOISY';
		}

		/**
		 * Right part of expression is the index
		 */
		$expr = new Expression($arrayAccess['right']);
		$exprIndex = $expr->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'int':
			case 'uint':
			case 'long':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_long(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $exprIndex->getCode() . ', ' . $flags . ' TSRMLS_CC);');
				break;
			case 'string':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_string(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $exprIndex->getCode() . '"), ' . $flags . ' TSRMLS_CC);');
				break;
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $expression);
				switch ($variableIndex->getType()) {
					case 'int':
					case 'uint':
					case 'long':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch_long(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ' TSRMLS_CC);');
						break;
					case 'string':
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ' TSRMLS_CC);');
						break;
					default:
						throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
				}
				break;
			default:
				throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $arrayAccess['right']);
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
