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

require 'Library/Operators/Arithmetical/BaseOperator.php';
require 'Library/Operators/Arithmetical/AddOperator.php';
require 'Library/Operators/Arithmetical/SubOperator.php';

/**
 * Expressions
 *
 * Represents an expression
 * Most language constructions in a language are expressions
 */
class Expression
{

	protected $_expression;

	protected $_expecting = true;

	protected $_expectingVariable;

	public function __construct(array $expression)
	{
		$this->_expression = $expression;
	}

	private function _addSlaches($str)
	{
		$str = str_replace('\\', "\\\\", $str);
		$str = str_replace('"', "\\\"", $str);
		$str = str_replace("\n", "\\n", $str);
		$str = str_replace("\r", "\\r", $str);
		$str = str_replace("\t", "\\t", $str);
		return $str;
	}

	/**
	 * Returns the original expression
	 *
	 * @return array
	 */
	public function getExpression()
	{
		return $this->_expression;
	}

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
	 * Checks if the returned value by the expression
	 * is expected to be assigned to an external symbol
	 *
	 * @return boolean
	 */
	public function isExpectingReturn()
	{
		return $this->_expecting;
	}

	public function getExpectingVariable()
	{
		return $this->_expectingVariable;
	}

	public function compileArray($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('array', $expression['left'], $expression);
	}

	public function compileEquals($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($right->getType()) {
					case 'int':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
					case 'null':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' == ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' == (int) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}
	}

	public function compileIdentical($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' == ' . $right->getCode() . ')', $expression);
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression);
								}
							case 'null':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression);
							default:
								throw new CompilerException("Error Processing Request", $expression);
						}
						break;
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' == ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' == (int) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}

	public function compileLess($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' < ' . $right->getCode() . ')', $expression);
					case 'double':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' < ' . $right->getCode() . ')', $expression);
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression);
								}
							default:
								throw new CompilerException("Error Processing Request", $expression);
						}
						break;
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' < ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' < (long) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'double':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(double)' . $left->getCode() . ' < ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' < ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}

	public function compileNot($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('bool', '', $expression);
	}

	public function compileIsset($expression, CompilationContext $compilationContext)
	{
		$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['left']['value']);

		switch ($expression['left']['type']) {
			case 'array-access':
				switch ($expression['left']['right']['type'])	{
					case 'string':
						return new CompiledExpression('int', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $expression['left']['right']['value'] . '"))', $expression);
					default:
						throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
				}
				break;
			default:
				throw new CompilerException('[' . $expression['left']['type'] . ']', $expression);
		}

		return new CompiledExpression('int', '', $expression);
	}

	/**
	 * Fetch is a special operator that checks if an expression 'isset' and then obtain the value
	 * without calculating the hash key twice
	 *
	 * @param array $expression
	 */
	public function compileFetch($expression, CompilationContext $compilationContext)
	{

		/**
		 * @TODO read variable, include new macros from 1.3.0
		 */

		$variable = $compilationContext->symbolTable->getVariableForWrite($expression['left']['value']);
		$variable->setIsInitialized(true);
		$variable->observeVariant($compilationContext);

		$evalVariable = $compilationContext->symbolTable->getVariableForRead($expression['right']['left']['value']);

		switch ($expression['right']['type']) {
			case 'array-access':
				switch ($expression['right']['right']['type'])	{
					case 'string':
						return new CompiledExpression('int', 'zephir_array_isset_string(' . $evalVariable->getName() . ', SS("' . $expression['right']['right']['value'] . '"))', $expression);
					default:
						throw new CompilerException('[' . $expression['right']['right']['type'] . ']', $expression);
				}
				break;
			default:
				throw new CompilerException('[' . $expression['right']['type'] . ']', $expression);
		}

		return new CompiledExpression('int', '', $expression);
	}

	/**
	 *
	 */
	public function propertyAccess($expression, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$propertyAccess = $expression;

		$expr = new Expression($propertyAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $expression);
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

		$property = $propertyAccess['right']['value'];

		/**
		 * Resolves the symbol that expects the value
		 */
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

		/**
		 * Variable that receives the method call must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompiledException("Cannot use variable: " . $symbolVariable->getType() . " to assign property value", $expression);
		}

		$compilationContext->headersManager->add('kernel/object');
		$codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Compiles foo[x] = {expr}
	 */
	public function arrayAccess($expression, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$arrayAccess = $expression;

		$expr = new Expression($arrayAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $expression);
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
		 * Resolves the symbol that expects the value
		 */
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

		/**
		 * Variable that receives the method call must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompiledException("Cannot use variable: " . $symbolVariable->getType() . " to assign array index", $expression);
		}

		/**
		 * Right part of expression is the index
		 */
		$expr = new Expression($arrayAccess['right']);
		$exprIndex = $expr->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'int':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_long(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $exprIndex->getCode() . ', PH_NOISY);');
				break;
			case 'string':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_string(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $exprIndex->getCode() . '"), PH_NOISY);');
				break;
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $expression);
				switch ($variableIndex->getType()) {
					case 'int':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch_long(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', PH_NOISY);');
						break;
					case 'string':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch_string(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $exprIndex->getCode() . '->str, ' . $exprIndex->getCode() . '->len, PH_NOISY);');
						break;
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', PH_NOISY);');
						break;
					default:
						throw new CompiledException("Variable type: " . $exprIndex->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
				}
				break;
			default:
				throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $arrayAccess['right']);
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Creates a new instance
	 *
	 */
	public function newInstance($expression, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$newExpr = $expression;

		/**
		 * Resolves the symbol that expects the value
		 */
		if ($this->_expecting) {
			if ($this->_expectingVariable) {
				$symbolVariable = $this->_expectingVariable;
				$symbolVariable->initVariant($compilationContext);
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
			}
		} else {
			$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
		}

		if (strtolower($newExpr['class']) == 'stdclass') {
			$codePrinter->output('object_init(' . $symbolVariable->getName() . ');');
		} else {
			if ($compilationContext->compiler->isClass($newExpr['class'])) {
				$classCe = strtolower(str_replace('\\', '_', $newExpr['class'])) . '_ce';
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $classCe . ');');
			} else {
				throw new CompilerException("Class " . $newExpr['class'] . " does not exist", $statement);
			}
		}

		if (strtolower($newExpr['class']) == 'stdclass') {
			if (isset($newExpr['parameters'])) {
				if (count($newExpr['parameters'])) {
					throw new CompilerException("Stdclasses don't receive parameters in its constructor", $statement);
				}
			}
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}

		/**
		 * Mark variable initialized
		 */
		$symbolVariable->setIsInitialized(true);

		/**
		 * Call the constructor
		 * For classes in the same extension we check if the class does implement a constructor
		 * For external classes we always assume the class does implement a constructor
		 */
		if ($compilationContext->compiler->isClass($newExpr['class'])) {
			$classDefinition = $compilationContext->compiler->getClassDefinition($newExpr['class']);
			if ($classDefinition->hasMethod("__construct")) {

				if (isset($newExpr['parameters'])) {
					$callExpr = new Expression(array(
						'variable' => $symbolVariable->getRealName(),
						'name' => '__construct',
						'parameters' => $newExpr['parameters'],
						'file' => $newExpr['file'],
						'line' => $newExpr['line'],
						'char' => $newExpr['char'],
					));
				} else {
					$callExpr = new Expression(array(
						'variable' => $symbolVariable->getRealName(),
						'name' => '__construct',
						'file' => $newExpr['file'],
						'line' => $newExpr['line'],
						'char' => $newExpr['char'],
					));
				}

				$methodCall = new MethodCall();
				$callExpr->setExpectReturn(false);
				$methodCall->compile($callExpr, $compilationContext);
			}
		} else {
			/**
			 * @TODO Check if the class has a constructor
			 */
			if (count($params)) {
				$codePrinter->output('zephir_call_method_p' . count($params) . '_noret(' . $variable . ', "__construct", ' . join(', ', $params) . ');');
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variable . ', "__construct");');
			}
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Resolves an expression
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$expression = $this->_expression;

		$type = $expression['type'];
		switch ($type) {

			case 'null':
				return new CompiledExpression('null', null, $expression);

			case 'int':
				return new CompiledExpression('int', $expression['value'], $expression);

			case 'double':
				return new CompiledExpression('double', $expression['value'], $expression);

			case 'bool':
				return new CompiledExpression('bool', $expression['value'], $expression);

			case 'string':
				return new CompiledExpression('string', $this->_addSlaches($expression['value']), $expression);

			case 'variable':
				return new CompiledExpression('variable', $expression['value'], $expression);

			case 'empty-array':
				return new CompiledExpression('empty-array', null, $expression);

			case 'array-access':
				return $this->arrayAccess($expression, $compilationContext);

			case 'property-access':
				return $this->propertyAccess($expression, $compilationContext);

			case 'fcall':
				return new CompiledExpression('null', null, $expression);

			case 'mcall':
				$methodCall = new MethodCall();
				return $methodCall->compile($this, $compilationContext);

			case 'isset':
				return $this->compileIsset($expression, $compilationContext);

			case 'fetch':
				return $this->compileFetch($expression, $compilationContext);

			case 'typeof':
				return new CompiledExpression('typeof', null, $expression);

			case 'array':
				return $this->compileArray($expression, $compilationContext);

			case 'new':
				return $this->newInstance($expression, $compilationContext);

			case 'equals':
				return $this->compileEquals($expression, $compilationContext);

			case 'not':
				return $this->compileNot($expression, $compilationContext);

			case 'identical':
				return $this->compileIdentical($expression, $compilationContext);

			case 'not-identical':
				return $this->compileIdentical($expression, $compilationContext);

			case 'less':
				return $this->compileLess($expression, $compilationContext);

			case 'add':
				$expr = new AddOperator();
				return $expr->compile($expression, $compilationContext);

			case 'sub':
				$expr = new SubOperator();
				return $expr->compile($expression, $compilationContext);

			default:
				throw new CompilerException("Unknown expression: " . $type, $expression);
		}
		//echo $type;
	}

}
