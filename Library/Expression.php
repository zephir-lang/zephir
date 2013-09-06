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

/* Base Operator */
require ZEPHIRPATH . 'Library/Operators/BaseOperator.php';

/* Arithmetical operators */
require ZEPHIRPATH . 'Library/Operators/Arithmetical/BaseOperator.php';
require ZEPHIRPATH . 'Library/Operators/Arithmetical/AddOperator.php';
require ZEPHIRPATH . 'Library/Operators/Arithmetical/SubOperator.php';
require ZEPHIRPATH . 'Library/Operators/Arithmetical/MulOperator.php';
require ZEPHIRPATH . 'Library/Operators/Arithmetical/DivOperator.php';
require ZEPHIRPATH . 'Library/Operators/Arithmetical/ModOperator.php';

/* Logical operators */
require ZEPHIRPATH . 'Library/Operators/Logical/BaseOperator.php';
require ZEPHIRPATH . 'Library/Operators/Logical/AndOperator.php';
require ZEPHIRPATH . 'Library/Operators/Logical/OrOperator.php';

/** Unary Operator */
require ZEPHIRPATH . 'Library/Operators/Unary/NotOperator.php';

/* Comparison operators */
require ZEPHIRPATH . 'Library/Operators/Comparison/BaseOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/IdenticalOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/NotIdenticalOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/EqualsOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/NotEqualsOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/LessOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/GreaterOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/LessEqualOperator.php';
require ZEPHIRPATH . 'Library/Operators/Comparison/GreaterEqualOperator.php';

/* Other operators */
require ZEPHIRPATH . 'Library/Operators/Other/ConcatOperator.php';

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

	protected $_readOnly = false;

	protected $_expectingVariable;

	/**
	 * Expression constructor
	 *
	 * @param array $expression
	 */
	public function __construct(array $expression)
	{
		$this->_expression = $expression;
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

	/**
	 * Returns the variable which is expected to return the
	 * result of the expression evaluation
	 *
	 * @return \Variable
	 */
	public function getExpectingVariable()
	{
		return $this->_expectingVariable;
	}

	/**
	 * Compiles an 'isset' operator
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compileIsset($expression, CompilationContext $compilationContext)
	{

		$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['left']['value'], $compilationContext, $expression['left']['left']);
		if ($variable->getType() != 'variable') {
			throw new CompiledException("Variable type: " . $variable->getType() . " cannot be used as array/object", $arrayAccess['left']);
		}

		$dynamicType = $variable->getDynamicType();
		if ($dynamicType != 'undefined' && $dynamicType != 'array' && $dynamicType != 'object') {
			$compilationContext->logger->warning('Possible attempt to use non array/object in isset operator', 'non-valid-increment', $statement);
		}

		$compilationContext->headersManager->add('kernel/array');

		switch ($expression['left']['type']) {
			case 'array-access':
				$expr = new Expression($expression['left']['right']);
				$resolvedExpr= $expr->compile($compilationContext);
				switch ($resolvedExpr->getType())	{
					case 'int':
					case 'long':
						return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $resolvedExpr->getCode() . ')', $expression['left']['right']);
					case 'string':
						return new CompiledExpression('bool', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $resolvedExpr->getCode() . '"))', $expression['left']['right']);
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expression['left']['right']);
						switch ($indexVariable->getType()) {
							case 'int':
							case 'long':
								return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $indexVariable->getName() . ')', $expression['left']['right']);
							case 'variable':
								return new CompiledExpression('bool', 'zephir_array_isset(' . $variable->getName() . ', ' . $indexVariable->getName() . ')', $expression['left']['right']);
							default:
								throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
						}
						break;
					default:
						throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
				}
				break;
			case 'property-access':
				/* @todo, implement this */
				return new CompiledExpression('bool', 'false', $expression);
			default:
				throw new CompilerException('[' . $expression['left']['type'] . ']', $expression);
		}

	}

	/**
	 * Fetch is a special operator that checks if an expression 'isset' and then obtain the value
	 * without calculating the hash key twice
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compileFetch($expression, CompilationContext $compilationContext)
	{

		$compilationContext->headersManager->add('kernel/array');

		$variable = $compilationContext->symbolTable->getVariableForWrite($expression['left']['value'], $compilationContext, $expression['left']);
		$variable->setIsInitialized(true);
		$variable->observeVariant($compilationContext);
		$variable->setDynamicType('undefined');

		$evalVariable = $compilationContext->symbolTable->getVariableForRead($expression['right']['left']['value'], $compilationContext, $expression['right']['left']);

		switch ($expression['right']['type']) {
			case 'array-access':
				$expr = new Expression($expression['right']['right']);
				$resolvedExpr = $expr->compile($compilationContext);
				switch ($resolvedExpr->getType())	{
					case 'string':
						return new CompiledExpression('bool', 'zephir_array_isset_string_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', SS("' . $expression['right']['right']['value'] . '"))', $expression);
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expression['right']['left']);
						switch ($indexVariable->getType()) {
							case 'int':
							case 'long':
							case 'uint':
								return new CompiledExpression('bool', 'zephir_array_isset_long_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $indexVariable->getName() . ')', $expression);
							case 'variable':
								return new CompiledExpression('bool', 'zephir_array_isset_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $indexVariable->getName() . ')', $expression);
							default:
								throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
						}
						break;
					default:
						throw new CompilerException('[' . $expression['right']['right']['type'] . ']', $expression);
				}
				break;
			default:
				throw new CompilerException('[' . $expression['right']['type'] . ']', $expression);
		}

	}

	/**
	 * Resolves the access to a property in an object
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
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
		 * At this point, we don't know the exact dynamic type fetched from the property
		 */
		$symbolVariable->setDynamicType('undefined');

		/**
		 * If the property is accessed on 'this', we check if the method does exist
		 */
		if ($variableVariable->getRealName() == 'this') {
			$classDefinition = $compilationContext->classDefinition;
			if (!$classDefinition->hasProperty($property)) {
				throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $expression);
			}
		}

		/**
		 * Variable that receives the method call must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompiledException("Cannot use variable: " . $symbolVariable->getType() . " to assign property value", $expression);
		}

		$compilationContext->headersManager->add('kernel/object');
		if ($variableVariable->getRealName() == 'this') {
			$codePrinter->output('zephir_read_property_this(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
		} else {
			$codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Access a static constant class
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function staticConstantAccess($expression, CompilationContext $compilationContext)
	{
		$compiler = $compilationContext->compiler;

		$className = $expression['left']['value'];

		/**
		 * Fetch the class definition according to the class where the constant
		 * is supposed to be declared
		 */
		if ($className != 'self' && $className != 'parent') {
			if (!$compiler->isClass($className)) {
				throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
			}
			$classDefinition = $compiler->getClassDefinition($className);
		} else {
			if ($className == 'self') {
				$classDefinition = $compilationContext->classDefinition;
			} else {
				if ($className == 'parent') {
					$classDefinition = $compilationContext->classDefinition;
					$extendsClass = $classDefinition->getExtendsClass();
					if (!$extendsClass) {
						throw new CompilerException('Cannot call method "' . $methodName . '" on parent because class ' .
							$classDefinition->getCompleteName() . ' does not extend any class', $expression);
					} else {
						$classDefinition = $classDefinition->getExtendsClassDefinition();
					}
				}
			}
		}

		/**
		 * Constants are resolved to its values in compile time
		 * so we need to check that they effectively do exist
		 */
		$constant = $expression['right']['value'];
		if (!$classDefinition->hasConstant($constant)) {
			throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a constant called: '" . $constant . "'", $expression);
		}

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

		/**
		 * Variable that receives property accesses must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompiledException("Cannot use variable: " . $symbolVariable->getType() . " to assign class constants", $expression);
		}

		$compilationContext->headersManager->add('kernel/object');
		$compilationContext->codePrinter->output('zephir_get_class_constant(' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ', SS("' . $constant . '") TSRMLS_CC);');
		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Compiles foo[x] = []
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function emptyArray($expression, CompilationContext $compilationContext)
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

		/**
		 * Variable that receives property accesses must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable: " . $symbolVariable->getName() . '(' . $symbolVariable->getType() . ") to create empty array", $expression);
		}

		/**
		 * Mark the variable as an 'array'
		 */
		$symbolVariable->setDynamicType('array');

		$compilationContext->codePrinter->output('array_init(' . $symbolVariable->getName() . ');');

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Compiles foo[x] = {expr}
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
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
		 * Variables that contain dynamic type data can be validated to be arrays
		 */
		$dynamicType = $variableVariable->getDynamicType();
		if ($dynamicType != 'undefined' && $dynamicType != 'array') {
			$compilationContext->logger->warning('Possible attempt to access index on a non-array dynamic variable', 'non-array-update', $expression);
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
		 * Variable that receives property accesses must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign array index", $expression);
		}

		/**
		 * At this point, we don't know the type fetched from the index
		 */
		$symbolVariable->setDynamicType('undefined');

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
					case 'uint':
					case 'long':
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
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
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

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Objects can only be instantiated in dynamical variables", $expression);
		}

		/**
		 * Mark variables as dynamic objects
		 */
		$symbolVariable->setDynamicType('object');

		if ($newExpr['class'] == 'self') {
			$className = $compilationContext->classDefinition->getCompleteName();
		} else {
			$className = $newExpr['class'];
		}

		/**
		 * stdclass don't have constructors
		 */
		if (strtolower($className) == 'stdclass') {
			$codePrinter->output('object_init(' . $symbolVariable->getName() . ');');
			$symbolVariable->setClassType('stdclass');
		} else {
			/**
			 * Classes inside the same extension
			 */
			if ($compilationContext->compiler->isClass($className)) {
				$classCe = strtolower(str_replace('\\', '_', $className)) . '_ce';
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $classCe . ');');
				$symbolVariable->setClassType($className);
			} else {

				/**
				 * Classes inside the same extension
				 */
				if (!class_exists($className, false)) {
					$compilationContext->logger->warning('Class "' . $newExpr['class'] . '" does not exist at compile time ', "nonexistant-class", $newExpr);
				}

				/**
				 * @TODO, check if variable is really internal
				 */

				$zendClassEntry = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
				$codePrinter->output($zendClassEntry->getName() . ' = zend_fetch_class(SL("' . $className . '"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $zendClassEntry->getName() . ');');
				$symbolVariable->setClassType($newExpr['class']);
			}
		}

		if (strtolower($className) == 'stdclass') {
			if (isset($newExpr['parameters'])) {
				if (count($newExpr['parameters'])) {
					throw new CompilerException("stdclass don't receive parameters in its constructor", $statement);
				}
			}
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}

		$symbolVariable->setDynamicType('object');

		/**
		 * Mark variable initialized
		 */
		$params = array();
		$symbolVariable->setIsInitialized(true);

		/**
		 * Call the constructor
		 * For classes in the same extension we check if the class does implement a constructor
		 * For external classes we always assume the class does implement a constructor
		 */
		$callConstructor = false;
		if ($compilationContext->compiler->isClass($className)) {
			$classDefinition = $compilationContext->compiler->getClassDefinition($className);
			if ($classDefinition->hasMethod("__construct")) {
				$callConstructor = true;
			}
		} else {
			if ($compilationContext->compiler->isInternalClass($className)) {
				$classDefinition = $compilationContext->compiler->getInternalClassDefinition($className);
				if ($classDefinition->hasMethod("__construct")) {
					$callConstructor = true;
				}
			}
		}

		if ($callConstructor) {

			if (isset($newExpr['parameters'])) {
				$callExpr = new Expression(array(
					'variable' => $symbolVariable->getRealName(),
					'name' => '__construct',
					'parameters' => $newExpr['parameters'],
					'call-type' => MethodCall::CALL_NORMAL,
					'file' => $newExpr['file'],
					'line' => $newExpr['line'],
					'char' => $newExpr['char'],
				));
			} else {
				$callExpr = new Expression(array(
					'variable' => $symbolVariable->getRealName(),
					'name' => '__construct',
					'call-type' => MethodCall::CALL_NORMAL,
					'file' => $newExpr['file'],
					'line' => $newExpr['line'],
					'char' => $newExpr['char'],
				));
			}

			$methodCall = new MethodCall();
			$callExpr->setExpectReturn(false);
			$methodCall->compile($callExpr, $compilationContext);
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Resolves an item to be added in an array
	 *
	 * @param \CompiledExpression $exprCompiled
	 * @param \CompilationContext $compilationContext
	 * @return \Variable
	 */
	public function getArrayValue($exprCompiled, CompilationContext $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		switch ($exprCompiled->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $exprCompiled->getCode() . ');');
				return $tempVar;
			case 'double':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $exprCompiled->getCode() . ');');
				return $tempVar;
			case 'bool':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', ' . $exprCompiled->getBooleanCode() . ');');
				return $tempVar;
			case 'null':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_NULL(' . $tempVar->getName() . ');');
				return $tempVar;
			case 'string':
				$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_STRING(' . $tempVar->getName() . ', "' . $exprCompiled->getCode() . '", 1);');
				return $tempVar;
			case 'variable':
				$itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());
				switch ($itemVariable->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar;
					case 'double':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar;
					case 'string':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_STRINGL(' . $tempVar->getName() . ', ' . $itemVariable->getName() . '->str, ' . $itemVariable->getName() . '->len, 1);');
						return $tempVar;
					case 'bool':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar;
					case 'variable':
						return $itemVariable;
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
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
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

		/**
		 * Arrays can only be variables
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable type: " . $symbolVariable->getType() . " as an array", $expression);
		}

		/*+
		 * Mark the variable as an array
		 */
		$symbolVariable->setDynamicType('array');

		$codePrinter = $compilationContext->codePrinter;

		$codePrinter->output('array_init(' . $symbolVariable->getName() . ');');
		foreach ($expression['left'] as $item) {
			if (isset($item['key'])) {
				$key = null;
				switch ($item['key']['type']) {
					case 'string':
						$expr = new Expression($item['value']);
						$resolvedExpr = $expr->compile($compilationContext);
						switch ($resolvedExpr->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), ' . $resolvedExpr->getCode() . ');');
								break;
							case 'double':
								$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), ' . $resolvedExpr->getCode() . ');');
								break;
							case 'bool':
								$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), ' . $resolvedExpr->getBooleanCode() . ');');
								break;
							case 'string':
								$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), SL("' . $resolvedExpr->getCode() . '"), 1);');
								break;
							case 'null':
								$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', SS("' . $resolvedExpr->getCode() . '"));');
								break;
							case 'variable':
								$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
								$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), &' . $valueVariable->getName() . ', PH_COPY | PH_SEPARATE);');
								if ($valueVariable->isTemporal()) {
									$valueVariable->setIdle(true);
								}
								break;
							default:
								throw new CompilerException("Invalid value type: " . $resolvedExpr->getType(), $item['value']);
						}
						break;
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$expr = new Expression($item['value']);
						$resolvedExpr = $expr->compile($compilationContext);
						switch ($resolvedExpr->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								$codePrinter->output('add_index_long(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getCode() . ');');
								break;
							case 'bool':
								$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getBooleanCode() . ');');
								break;
							case 'double':
								$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getCode() . ');');
								break;
							case 'null':
								$codePrinter->output('add_index_null(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ');');
								break;
							case 'string':
								$codePrinter->output('add_index_stringl(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', SL("' . $resolvedExpr->getCode() . '"), 1);');
								break;
							case 'variable':
								$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
								$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $valueVariable->getName() . ', PH_COPY | PH_SEPARATE);');
								if ($valueVariable->isTemporal()) {
									$valueVariable->setIdle(true);
								}
								break;
							default:
								throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
						}
						break;
					case 'variable':
						$variableVariable = $compilationContext->symbolTable->getVariableForRead($item['key']['value'], $compilationContext, $item['key']);
						switch ($variableVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								$expr = new Expression($item['value']);
								$resolvedExpr = $expr->compile($compilationContext);
								switch ($resolvedExpr->getType()) {
									case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
										$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getCode() . ');');
										break;
									case 'bool':
										$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getBooleanCode() . ';');
										break;
									case 'double':
										$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getCode() . ');');
										break;
									case 'null':
										$codePrinter->output('add_index_null(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ');');
										break;
									case 'string':
										$codePrinter->output('add_index_stringl(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', SL("' . $resolvedExpr->getCode() . '"), 1);');
										break;
									case 'variable':
										$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
										$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $valueVariable->getName() . ', PH_COPY | PH_SEPARATE);');
										if ($valueVariable->isTemporal()) {
											$valueVariable->setIdle(true);
										}
										break;
									default:
										throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
								}
								break;
							case 'string':
								$expr = new Expression($item['value']);
								$resolvedExpr = $expr->compile($compilationContext);
								switch ($resolvedExpr->getType()) {
									case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
										$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $resolvedExpr->getCode() . ');');
										break;
									case 'double':
										$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $resolvedExpr->getCode() . ');');
										break;
									case 'bool':
										$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $resolvedExpr->getBooleanCode() . ');');
										break;
									case 'string':
										$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1, SL("' . $resolvedExpr->getCode() . '"), 1);');
										break;
									case 'null':
										$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1);');
										break;
									case 'variable':
										$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
										$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), &' . $valueVariable->getName() . ', PH_COPY | PH_SEPARATE);');
										if ($valueVariable->isTemporal()) {
											$valueVariable->setIdle(true);
										}
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
				$expr = new Expression($item['value']);
				$resolvedExpr = $expr->compile($compilationContext);
				$itemVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_append(&' . $symbolVariable->getName() . ', ' . $itemVariable->getName() . ', 0);');
				if ($itemVariable->isTemporal()) {
					$itemVariable->setIdle(true);
				}
			}
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

	/**
	 * Converts a value into another
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compileTypeHint($expression, CompilationContext $compilationContext)
	{

		$expr = new Expression($expression['right']);
		$resolved = $expr->compile($compilationContext);

		if ($resolved->getType() != 'variable') {
			throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
		}

		$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $expression);
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
		}

		$symbolVariable->setDynamicType('object');
		$symbolVariable->setClassType($expression['left']['value']);

		return $resolved;
	}

	/**
	 * Converts a value into another
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compileCast($expression, CompilationContext $compilationContext)
	{

		$expr = new Expression($expression['right']);
		$resolved = $expr->compile($compilationContext);

		switch ($expression['left']) {
			case 'int':
				switch ($resolved->getType()) {
					case 'double':
						return new CompiledExpression('int', '(int) ' . $resolved->getCode(), $expression);
					case 'bool':
						return new CompiledExpression('int', $resolved->getBooleanCode(), $expression);
					case 'variable':
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $expression);
						return new CompiledExpression('int', 'zephir_get_numberval(' . $symbolVariable->getName() . ')', $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'bool':
				switch ($resolved->getType()) {
					case 'variable':
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $expression);
						return new CompiledExpression('int', 'zephir_get_boolval(' . $symbolVariable->getName() . ')', $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'string':
				switch ($resolved->getType()) {
					case 'variable':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('string', $compilationContext, $expression);
						$symbolVariable->setMustInitNull(true);
						$compilationContext->codePrinter->output('zephir_get_strval(' . $resolved->getCode() . ', &' . $symbolVariable->getName() . ');');
						return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'char':
				switch ($resolved->getType()) {
					case 'variable':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext, $expression);
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $expression);
						$compilationContext->codePrinter->output($tempVariable->getName() . ' = (char) zephir_get_intval(' . $symbolVariable->getName() . ');');
						return new CompiledExpression('variable', $tempVariable->getName(), $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			default:
				throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
		}

	}

	/**
	 * Resolves a PHP constant value into C-code
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compileConstant($expression, CompilationContext $compilationContext)
	{

		$exists = true;
		if (!defined($expression['value'])) {
			$compilationContext->logger->warning("Constant \"" . $expression['value'] . "\" does not exist at compile time", "nonexistant-constant");
			$exists = false;
		}

		if ($exists) {
			$value = constant($expression['value']);
			switch (gettype($value)) {
				case 'integer':
					return new CompiledExpression('int', $value, $expression);
			}
			return new CompiledExpression(gettype($value), $value, $expression);
		}

		return new CompiledExpression('null', null, $expression);
	}

	/**
	 * Resolves an expression
	 *
	 * @param \CompilationContext $compilationContext
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
				if (ctype_digit($expression['value'])) {
					return new CompiledExpression('int', $expression['value'], $expression);
				}
				return new CompiledExpression('string', Utils::addSlaches($expression['value']), $expression);

			case 'char':
				if (strlen($expression['value']) > 2) {
					throw new CompilerException("Invalid char literal: " . $expression['value'], $expression);
				}
				return new CompiledExpression('char', $expression['value'], $expression);

			case 'variable':
				return new CompiledExpression('variable', $expression['value'], $expression);

			case 'constant':
				return $this->compileConstant($expression, $compilationContext);

			case 'empty-array':
				return $this->emptyArray($expression, $compilationContext);

			case 'array-access':
				return $this->arrayAccess($expression, $compilationContext);

			case 'property-access':
				return $this->propertyAccess($expression, $compilationContext);

			case 'static-constant-access':
				return $this->staticConstantAccess($expression, $compilationContext);

			case 'fcall':
				$functionCall = new FunctionCall();
				return $functionCall->compile($this, $compilationContext);

			case 'mcall':
				$methodCall = new MethodCall();
				return $methodCall->compile($this, $compilationContext);

			case 'scall':
				$staticCall = new StaticCall();
				return $staticCall->compile($this, $compilationContext);

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

			case 'not':
				$expr = new NotOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'equals':
				$expr = new EqualsOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'not-equals':
				$expr = new NotEqualsOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'identical':
				$expr = new IdenticalOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'not-identical':
				$expr = new NotIdenticalOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'greater':
				$expr = new GreaterOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'less':
				$expr = new LessOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'less-equal':
				$expr = new LessEqualOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'greater-equal':
				$expr = new GreaterEqualOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'add':
				$expr = new AddOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'sub':
				$expr = new SubOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'mul':
				$expr = new MulOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'div':
				$expr = new DivOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'mod':
				$expr = new ModOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'and':
				$expr = new AndOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'or':
				$expr = new OrOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'concat':
				$expr = new ConcatOperator();
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'list':
				if ($expression['left']['type'] == 'list') {
					$compilationContext->logger->warning("Unnecessary extra parentheses", "extra-parentheses");
				}
				$numberPrints = $compilationContext->codePrinter->getNumberPrints();
				$expr = new Expression($expression['left']);
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				$resolved = $expr->compile($compilationContext);
				if (($compilationContext->codePrinter->getNumberPrints() - $numberPrints) <= 1) {
					if (strpos($resolved->getCode(), ' ') !== false) {
						return new CompiledExpression($resolved->getType(), '(' . $resolved->getCode() . ')', $expression);
					}
				}
				return $resolved;

			case 'cast':
				return $this->compileCast($expression, $compilationContext);

			case 'type-hint':
				return $this->compileTypeHint($expression, $compilationContext);

			/**
			 * @TODO implement this
			 */
			case 'require':
			case 'instanceof':
			case 'typeof':
			case 'static-property-access':
				return new CompiledExpression('null', null, $expression);

			default:
				throw new CompilerException("Unknown expression: " . $type, $expression);
		}
	}

}
