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

/**
 * PropertyDynamicAccess
 *
 * Resolves expressions that read properties with a dynamic variable as property
 */
class PropertyDynamicAccess
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
	 * Resolves the access to a property in an object
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		$propertyAccess = $expression;

		$expr = new Expression($propertyAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $compilationContext, $expression);
				switch ($variableVariable->getType()) {
					case 'variable':
						break;
					default:
						throw new CompiledException("Variable type: " . $variableVariable->getType() . " cannot be used as object", $propertyAccess['left']);
				}
				break;
			default:
				throw new CompiledException("Cannot use expression: " . $exprVariable->getType() . " as an object", $propertyAccess['left']);
		}

		$propertyVariable = $compilationContext->symbolTable->getVariableForRead($propertyAccess['right']['value'], $compilationContext, $expression);
		switch ($variableVariable->getType()) {
			case 'variable':
				break;
			default:
				throw new CompiledException("Variable type: " . $variableVariable->getType() . " cannot be used as object", $propertyAccess['left']);
		}

		/**
		 * Resolves the symbol that expects the value
		 */
		if ($this->_expecting) {
			if ($this->_expectingVariable) {
				$symbolVariable = $this->_expectingVariable;
				if ($symbolVariable->getName() != 'return_value') {
					$symbolVariable->observeVariant($compilationContext);
				} else {
					$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
				}
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
			}
		}

		/**
		 * Variable that receives a property value must be polymorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign property value", $expression);
		}

		/**
		 * At this point, we don't know the exact dynamic type fetched from the property
		 */
		$symbolVariable->setDynamicTypes('undefined');

		$compilationContext->headersManager->add('kernel/object');
		$codePrinter->output('zephir_read_property_zval(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $propertyVariable->getName() . ', PH_NOISY_CC);');

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
