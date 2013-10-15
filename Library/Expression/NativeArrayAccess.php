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

		/**
		 * Resolve the left part of the expression
		 */
		$expr = new Expression($arrayAccess['left']);
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
						break;
					default:
						throw new CompilerException("Variable type: " . $variableVariable->getType() . " cannot be used as array", $arrayAccess['left']);
				}
				break;
			default:
				throw new CompilerException("Cannot use expression: " . $exprVariable->getType() . " as an array", $arrayAccess['left']);
		}

		if ($variableVariable->hasAnyDynamicType('unknown')) {
			throw new CompilerException("Cannot use non-initialized variable as an array", $statement);
		}

		/**
		 * Trying to use a non-object dynamic variable as object
		 */
		if ($variableVariable->hasDifferentDynamicType(array('undefined', 'array', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to access array-index on a non-array dynamic variable', 'non-array-access', $statement);
		}

		/**
		 * Resolves the symbol that expects the value
		 */
		$readOnly = false;
		if ($this->_readOnly) {
			if ($this->_expecting) {
				if ($this->_expectingVariable) {

					$symbolVariable = $this->_expectingVariable;

					/**
					 * If a variable is assigned once in the method, we try to promote it
					 * to a read only variable
					 */
					if ($symbolVariable->getName() != 'return_value') {
						$numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
						if ($numberMutations == 1) {
							if ($symbolVariable->getNumberMutations() == $numberMutations) {
								$symbolVariable->setMemoryTracked(false);
								$readOnly = true;
							}
						}
					}

					/**
					 * Variable is not read-only or it wasn't promoted
					 */
					if (!$readOnly) {
						if ($symbolVariable->getName() != 'return_value') {
							$symbolVariable->observeVariant($compilationContext);
							$this->_readOnly = false;
						} else {
							$symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, $expression);
						}
					}

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

					/**
					 * If a variable is assigned once in the method, we try to promote it
					 * to a read only variable
					 */
					if ($symbolVariable->getName() != 'return_value') {
						$numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
						if ($numberMutations == 1) {
							if ($symbolVariable->getNumberMutations() == $numberMutations) {
								$symbolVariable->setMemoryTracked(false);
								$readOnly = true;
							}
						}
					}

					/**
					 * Variable is not read-only or it wasn't promoted
					 */
					if (!$readOnly) {
						if ($symbolVariable->getName() != 'return_value') {
							$symbolVariable->observeVariant($compilationContext);
							$this->_readOnly = false;
						} else {
							$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
						}
					}

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
		$symbolVariable->setDynamicTypes('undefined');

		if ($this->_readOnly || $readOnly) {
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
