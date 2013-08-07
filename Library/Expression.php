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

				$variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);

				switch ($right->getType()) {
					case 'int':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
					case 'null':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression);
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'ZEPHIR_IS_EQUALS(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
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

				$variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' == ' . $right->getCode() . ')', $expression['left']);
					case 'bool':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' == ' . $right->getCode() . ')', $expression['left']);
							case 'bool':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' == ' . $right->getBooleanCode() . ')', $expression['left']);
							case 'null':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' == 0)', $expression['left']);
							default:
								throw new CompilerException("Error Processing Request", $expression['left']);
						}
						break;
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression['left']);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression['left']);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression['left']);
								}
							case 'null':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression['left']);
							default:
								throw new CompilerException("Error Processing Request", $expression['left']);
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

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value'], $compilationContext, $expression['left']);

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
		$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['left']['value'], $compilationContext, $expression['left']['left']);

		switch ($expression['left']['type']) {
			case 'array-access':
				switch ($expression['left']['right']['type'])	{
					case 'string':
						return new CompiledExpression('int', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $expression['left']['right']['value'] . '"))', $expression);
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($expression['left']['right']['value'], $compilationContext, $expression['left']['right']);
						return new CompiledExpression('int', 'zephir_array_isset(' . $variable->getName() . ', ' . $indexVariable->getName() . ')', $expression);
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

		$evalVariable = $compilationContext->symbolTable->getVariableForRead($expression['right']['left']['value'], $compilationContext, $expression['right']['left']);

		switch ($expression['right']['type']) {
			case 'array-access':
				switch ($expression['right']['right']['type'])	{
					case 'string':
						return new CompiledExpression('int', 'zephir_array_isset_string(' . $evalVariable->getName() . ', SS("' . $expression['right']['right']['value'] . '"))', $expression);
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($expression['right']['right']['value'], $compilationContext, $expression['right']['left']);
						return new CompiledExpression('int', 'zephir_array_isset(' . $evalVariable->getName() . ', ' . $indexVariable->getName() . ')', $expression);
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
	 * @
	 */
	public function propertyAccess($expression, CompilationContext $compilationContext)
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
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $expression);
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
						throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
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
	 * Resolves an item to be added in an array
	 */
	public function getArrayValue($item, CompilationContext $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		$expression = new Expression($item['value']);
		$exprCompiled = $expression->compile($compilationContext);
		switch ($exprCompiled->getType()) {
			case 'int':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $item['value']['value'] . ');');
				return $tempVar->getName();
			case 'double':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $item['value']['value'] . ');');
				return $tempVar->getName();
			case 'bool':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				if ($item['value']['value'] == 'true') {
					$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', 1);');
				} else {
					$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', 0);');
				}
				return $tempVar->getName();
			case 'null':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_NULL(' . $tempVar->getName() . ');');
				return $tempVar->getName();
			case 'string':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_STRING(' . $tempVar->getName() . ', "' . $item['value']['value'] . '", 1);');
				return $tempVar->getName();
			case 'variable':
				$itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $item);
				switch ($itemVariable->getType()) {
					case 'int':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar->getName();
					case 'double':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar->getName();
					case 'string':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_STRING(' . $tempVar->getName() . ', ' . $itemVariable->getName() . '->str, 1);');
						return $tempVar->getName();
					case 'bool':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar->getName();
					case 'variable':
						return $itemVariable->getName();
					default:
						throw new CompilerException("Unknown " . $itemVariable->getType(), $item);
				}
				break;
			default:
				throw new CompilerException("Unknown", $item);
		}
	}

	/**
	 * Compiles an array initialization
	 */
	public function compileArray($expression, CompilationContext $compilationContext)
	{

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

		$codePrinter = $compilationContext->codePrinter;

		$codePrinter->output('array_init(' . $symbolVariable->getName() . ');');
		foreach ($expression['left'] as $item) {
			if (isset($item['key'])) {
				$key = null;
				switch ($item['key']['type']) {
					case 'string':
						switch ($item['value']['type']) {
							case 'int':
								$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), ' . $item['value']['value'] . ');');
								break;
							case 'double':
								$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), ' . $item['value']['value'] . ');');
								break;
							case 'bool':
								if ($item['value']['value'] == 'true') {
									$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), 1);');
								} else {
									$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), 0);');
								}
								break;
							case 'string':
								$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), SL("' . $item['value']['value'] . '"), 1);');
								break;
							case 'null':
								$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"));');
								break;
							default:
								throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
						}
						break;
					case 'int':
						switch ($item['value']['type']) {
							case 'int':
								$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
								break;
							case 'bool':
								if ($item['value']['value'] == 'true') {
									$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', 1);');
								} else {
									$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', 0);');
								}
								break;
							case 'double':
								$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
								break;
							case 'null':
								$codePrinter->output('add_index_null(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ');');
								break;
							case 'string':
								$codePrinter->output('add_index_stringl(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', SL("' . $item['value']['value'] . '"), 1);');
								break;
							case 'variable':
								$value = $this->getArrayValue($item, $compilationContext);
								$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $value . ', PH_COPY | PH_SEPARATE);');
								break;
							default:
								throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
						}
						break;
					case 'variable':
						$variableVariable = $compilationContext->symbolTable->getVariableForRead($item['key']['value'], $compilationContext, $item['key']);
						switch ($variableVariable->getType()) {
							case 'int':
								switch ($item['value']['type']) {
									case 'int':
										$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
										break;
									case 'bool':
										if ($item['value']['value'] == 'true') {
											$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', 1);');
										} else {
											$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', 0);');
										}
										break;
									case 'double':
										$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
										break;
									case 'null':
										$codePrinter->output('add_index_null(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ');');
										break;
									case 'string':
										$codePrinter->output('add_index_stringl(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', SL("' . $item['value']['value'] . '"), 1);');
										break;
									case 'variable':
										$value = $this->getArrayValue($item, $compilationContext);
										$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $value . ', PH_COPY | PH_SEPARATE);');
										break;
									default:
										throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
								}
								break;
							case 'string':
								switch ($item['value']['type']) {
									case 'int':
										$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $item['value']['value'] . ');');
										break;
									case 'double':
										$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $item['value']['value'] . ');');
										break;
									case 'bool':
										if ($item['value']['value'] == 'true') {
											$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, 1);');
										} else {
											$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, 0);');
										}
										break;
									case 'string':
										$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1, SL("' . $item['value']['value'] . '"), 1);');
										break;
									case 'null':
										$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1);');
										break;
									default:
										throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
								}
								break;
							default:
								throw new CompilerException("Cannot use variable type: " . $variableVariable->getType(). " as array index", $item['key']);
						}
						break;
					default:
						throw new CompilerException("Invalid key type: " . $item['key']['type'], $item['key']);
				}
			} else {
				$item = $this->getArrayValue($item, $compilationContext);
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_append(&' . $symbolVariable->getName() . ', ' . $item . ', 0);');
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
				$functionCall = new FunctionCall();
				return $functionCall->compile($this, $compilationContext);

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

			case 'concat':
				return new CompiledExpression('null', null, $expression);

			default:
				throw new CompilerException("Unknown expression: " . $type, $expression);
		}
	}

}
