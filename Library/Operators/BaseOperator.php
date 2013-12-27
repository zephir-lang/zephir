<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

class BaseOperator
{

	protected $_expecting = true;

	protected $_readOnly = false;

	protected $_literalOnly = true;

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
	 * Returns the expected variable for assignment or creates a temporary variable to
	 * store the result
	 *
	 * @param CompilationContext $compilationContext
	 * @param array $expression
	 */
	public function getExpected(CompilationContext $compilationContext, $expression)
	{
		$isExpecting = $this->_expecting;
		$symbolVariable = $this->_expectingVariable;

		if ($isExpecting) {
			if (is_object($symbolVariable)) {
				if ($symbolVariable->getType() == 'variable') {
					$symbolVariable->initVariant($compilationContext);
				} else {
					if (!$this->_readOnly) {
						if (!$this->_literalOnly) {
							$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
						} else {
							$symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
						}
					} else {
						$symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					}
				}
			} else {
				if (!$this->_readOnly) {
					if (!$this->_literalOnly) {
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					} else {
						$symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
					}
				} else {
					$symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
				}
			}
		}
		return $symbolVariable;
	}

	/**
	 * Returns the expected variable for assignment or creates a temporary variable to
	 * store the result, if a temporary variable is created it use whose body is only freed
	 * on every iteration
	 *
	 * @param CompilationContext $compilationContext
	 * @param array $expression
	 */
	public function getExpectedComplexLiteral(CompilationContext $compilationContext, $expression)
	{
		$isExpecting = $this->_expecting;
		$symbolVariable = $this->_expectingVariable;

		if ($isExpecting) {
			if (is_object($symbolVariable)) {
				if ($symbolVariable->getType() == 'variable') {
					$symbolVariable->initVariant($compilationContext);
				} else {
					if (!$this->_readOnly) {
						$symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
					} else {
						$symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					}
				}
			} else {
				if (!$this->_readOnly) {
					$symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite('variable', $compilationContext, $expression);
				} else {
					$symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
				}
			}
		}

		return $symbolVariable;
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
	 * Checks if the result of the evaluated expression is read only
	 *
	 * @return boolean
	 */
	public function isReadOnly()
	{
		return $this->_readOnly;
	}
}