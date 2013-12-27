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
require ZEPHIRPATH . 'Library/Operators/Other/FetchOperator.php';
require ZEPHIRPATH . 'Library/Operators/Other/IssetOperator.php';
require ZEPHIRPATH . 'Library/Operators/Other/EmptyOperator.php';
require ZEPHIRPATH . 'Library/Operators/Other/CloneOperator.php';

/* Expression Resolving */
require ZEPHIRPATH . 'Library/Expression/Constants.php';
require ZEPHIRPATH . 'Library/Expression/PropertyAccess.php';
require ZEPHIRPATH . 'Library/Expression/PropertyDynamicAccess.php';
require ZEPHIRPATH . 'Library/Expression/StaticConstantAccess.php';
require ZEPHIRPATH . 'Library/Expression/NativeArrayAccess.php';
require ZEPHIRPATH . 'Library/Expression/StaticPropertyAccess.php';

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
	 * Compiles foo = []
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
		$symbolVariable->setDynamicTypes('array');

		$compilationContext->codePrinter->output('array_init(' . $symbolVariable->getName() . ');');

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
		$symbolVariable->setDynamicTypes('object');

		$dynamic = false;
		if ($newExpr['class'] == 'self') {
			$className = $compilationContext->classDefinition->getCompleteName();
		} else {
			if (substr($newExpr['class'], 0, 1) == '\\') {
				$className = substr($newExpr['class'], 1);
			} else {
				$className = $newExpr['class'];
				$dynamic = $newExpr['dynamic'];
			}
		}

		if (!$className) {
			throw new CompilerException("A class name is required to instantiate the object", $expression);
		}

		/**
		 * stdclass don't have constructors
		 */
		if (strtolower($className) == 'stdclass') {
			$codePrinter->output('object_init(' . $symbolVariable->getName() . ');');
			$symbolVariable->setClassTypes('stdclass');
		} else {
			/**
			 * Classes inside the same extension
			 */
			if ($compilationContext->compiler->isClass($className)) {

				$classDefinition = $compilationContext->compiler->getClassDefinition($className);

				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ');');
				$symbolVariable->setClassTypes($className);
			} else {

				/**
				 * Classes inside the same extension
				 */
				if (!class_exists($className, false)) {
					$compilationContext->logger->warning('Class "' . $className . '" does not exist at compile time ', "nonexistent-class", $newExpr);
				}

				/**
				 * @TODO, check if the variable is really internal
				 */
				$zendClassEntry = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);

				$classNameToFetch = $dynamic ? 'Z_STRVAL_P('.$className.'), Z_STRLEN_P('.$className.')' : 'SL("' . Utils::addSlashes($className, true) . '")';
				$codePrinter->output($zendClassEntry->getName() . ' = zend_fetch_class('.$classNameToFetch.', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $zendClassEntry->getName() . ');');
				$symbolVariable->setClassTypes($newExpr['class']);
			}
		}

		if (strtolower($className) == 'stdclass') {
			if (isset($newExpr['parameters'])) {
				if (count($newExpr['parameters'])) {
					throw new CompilerException("stdclass does not receive parameters in its constructor", $expression);
				}
			}
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}

		$symbolVariable->setDynamicTypes('object');

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
			if ($classDefinition->getType() != 'class') {
				throw new CompilerException("Only classes can be instantiated", $expression);
			}
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
					'variable' => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
					'name' => '__construct',
					'parameters' => $newExpr['parameters'],
					'call-type' => MethodCall::CALL_NORMAL,
					'file' => $newExpr['file'],
					'line' => $newExpr['line'],
					'char' => $newExpr['char'],
				));
			} else {
				$callExpr = new Expression(array(
					'variable' => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
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
				if ($exprCompiled->getCode() == 'true') {
					return new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
				} else {
					return new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
				}
			case 'null':
				return new GlobalConstant('ZEPHIR_GLOBAL(global_null)');
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
					case 'bool':
						$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
						return $tempVar;
					case 'string':
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
		$symbolVariable->setDynamicTypes('array');

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
								$compilationContext->headersManager->add('kernel/array');
								if ($resolvedExpr->getCode() == 'true') {
									$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SL("' . $item['key']['value'] . '"), &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE);');
								} else {
									$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SL("' . $item['key']['value'] . '"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);');
								}
								break;
							case 'string':
								$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', SS("' . $item['key']['value'] . '"), SL("' . $resolvedExpr->getCode() . '"), 1);');
								break;
							case 'null':
								$compilationContext->headersManager->add('kernel/array');
								$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SL("' . $item['key']['value'] . '"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);');
								break;
							case 'variable':
								$compilationContext->headersManager->add('kernel/array');
								$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
								$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SL("' . $item['key']['value'] . '"), &' . $valueVariable->getName() . ', PH_COPY | PH_SEPARATE);');
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
								$compilationContext->headersManager->add('kernel/array');
								$codePrinter->output('add_index_bool(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getBooleanCode() . ');');
								if ($resolvedExpr->getCode() == 'true') {
									$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &ZEPHIR_GLOBAL(global_true), PH_COPY);');
								} else {
									$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &ZEPHIR_GLOBAL(global_false), PH_COPY);');
								}
								break;
							case 'double':
								$codePrinter->output('add_index_double(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', ' . $resolvedExpr->getCode() . ');');
								break;
							case 'null':
								$compilationContext->headersManager->add('kernel/array');
								$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &ZEPHIR_GLOBAL(global_null), PH_COPY);');
								break;
							case 'string':
								$codePrinter->output('add_index_stringl(' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', SL("' . $resolvedExpr->getCode() . '"), 1);');
								break;
							case 'variable':
								$compilationContext->headersManager->add('kernel/array');
								$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
								$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $valueVariable->getName() . ', PH_COPY);');
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
										$compilationContext->headersManager->add('kernel/array');
										$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
										$codePrinter->output('zephir_array_update_long(&' . $symbolVariable->getName() . ', ' . $item['key']['value'] . ', &' . $valueVariable->getName() . ', PH_COPY);');
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
										$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode() . ');');
										break;
									case 'double':
										$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode() . ');');
										break;
									case 'bool':
										$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getBooleanCode() . ');');
										break;
									case 'string':
										$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . ') + 1, SL("' . $resolvedExpr->getCode() . '"), 1);');
										break;
									case 'null':
										$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . ') + 1);');
										break;
									case 'variable':
										$compilationContext->headersManager->add('kernel/array');
										$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
										$codePrinter->output('zephir_array_update_string(&' . $symbolVariable->getName() . ', SL("' . $item['key']['value'] . '"), &' . $valueVariable->getName() . ', PH_COPY);');
										if ($valueVariable->isTemporal()) {
											$valueVariable->setIdle(true);
										}
										break;
									default:
										throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
								}
								break;
							case 'variable':
								$expr = new Expression($item['value']);
								$resolvedExpr = $expr->compile($compilationContext);
								switch ($resolvedExpr->getType()) {
									/*case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
										$codePrinter->output('add_assoc_long_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode() . ');');
										break;
									case 'double':
										$codePrinter->output('add_assoc_double_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode() . ');');
										break;
									case 'bool':
										$codePrinter->output('add_assoc_bool_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getBooleanCode() . ');');
										break;
									case 'string':
										$codePrinter->output('add_assoc_stringl_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . ') + 1, SL("' . $resolvedExpr->getCode() . '"), 1);');
										break;
									case 'null':
										$codePrinter->output('add_assoc_null_ex(' . $symbolVariable->getName() . ', Z_STRVAL_P(' . $item['key']['value'] . '), Z_STRLEN_P(' . $item['key']['value'] . ') + 1);');
										break;*/
									case 'variable':
										$compilationContext->headersManager->add('kernel/array');
										$valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
										$codePrinter->output('zephir_array_update_zval(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', &' . $valueVariable->getName() . ', PH_COPY);');
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
				$codePrinter->output('zephir_array_fast_append(' . $symbolVariable->getName() . ', ' . $itemVariable->getName() . ');');
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
		$expr->setReadOnly(true);
		$resolved = $expr->compile($compilationContext);

		if ($resolved->getType() != 'variable') {
			throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
		}

		$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
		}

		$symbolVariable->setDynamicTypes('object');
		$symbolVariable->setClassTypes($expression['left']['value']);

		return $resolved;
	}

	public function compileInstanceOf($expression, CompilationContext $compilationContext)
	{

		$left = new Expression($expression['left']);
		$resolved = $left->compile($compilationContext);

		if ($resolved->getType() != 'variable') {
			throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
		}

		$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
		}

		$right = new Expression($expression['right']);
		$resolved = $right->compile($compilationContext);
		$resolvedVariable = $resolved->getCode();
		switch ($resolved->getType()) {
			case 'string':
				$code = 'SL("' . trim($resolvedVariable, "\\") . '")';
				break;
			default:
				switch ($resolved->getType()) {
					case 'variable':
						if (!$compilationContext->symbolTable->hasVariable($resolvedVariable)) {
							$code = 'SL("' . Utils::addSlashes(trim($resolvedVariable, "\\"), true) . '")';
						}
						break;
					case 'property-access':
					case 'array-access':
						$compilationContext->headersManager->add('kernel/operators');
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('string', $compilationContext);
						$tempVariable->setMustInitNull(true);
						$tempVariableName = $tempVariable->getName();
						$compilationContext->codePrinter->output('zephir_get_strval(' . $tempVariableName . ', ' . $resolvedVariable . ');');
						$code = 'Z_STRVAL_P(' . $tempVariableName . '), Z_STRLEN_P(' . $tempVariableName . ')';
						break;
					default:
						throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
				}
		}

		$compilationContext->headersManager->add('kernel/object');
		return new CompiledExpression('bool', 'zephir_is_instance_of(' . $symbolVariable->getName() . ', ' . $code . ' TSRMLS_CC)', $expression);
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
					case 'int':
						return new CompiledExpression('int', $resolved->getCode(), $expression);
					case 'double':
						return new CompiledExpression('int', '(int) ' . $resolved->getCode(), $expression);
					case 'bool':
						return new CompiledExpression('int', $resolved->getBooleanCode(), $expression);
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
						if ($symbolVariable->isTemporal()) {
							$symbolVariable->setIdle(true);
						}
						switch ($symbolVariable->getType()) {
							case 'int':
								return new CompiledExpression('int', $symbolVariable->getName(), $expression);
							case 'double':
								return new CompiledExpression('int', '(int) (' . $symbolVariable->getName() . ')', $expression);
							case 'variable':
								return new CompiledExpression('int', 'zephir_get_intval(' . $symbolVariable->getName() . ')', $expression);
							default:
								throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
						}
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'double':
				switch ($resolved->getType()) {
					case 'double':
						return new CompiledExpression('int', $resolved->getCode(), $expression);
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
						return new CompiledExpression('double', 'zephir_get_doubleval(' . $symbolVariable->getName() . ')', $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'bool':
				switch ($resolved->getType()) {
					case 'bool':
						return new CompiledExpression('bool', $resolved->getCode(), $expression);
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
						if ($symbolVariable->isTemporal()) {
							$symbolVariable->setIdle(true);
						}
						switch ($symbolVariable->getType()) {
							case 'variable':
								return new CompiledExpression('bool', 'zephir_get_boolval(' . $symbolVariable->getName() . ')', $expression);
							default:
								throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
						}
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'string':
				switch ($resolved->getType()) {
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						$symbolVariable = $compilationContext->symbolTable->getTempVariable('string', $compilationContext, $expression);
						$symbolVariable->setMustInitNull(true);
						$symbolVariable->setIsInitialized(true);
						$compilationContext->codePrinter->output('zephir_get_strval(' . $symbolVariable->getName() . ', ' . $resolved->getCode() . ');');
						if ($symbolVariable->isTemporal()) {
							$symbolVariable->setIdle(true);
						}
						return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
					default:
						throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
				}
				break;
			case 'char':
				switch ($resolved->getType()) {
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext, $expression);
						$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
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
			case 'integer':
				return new CompiledExpression('int', $expression['value'], $expression);

			case 'double':
				return new CompiledExpression('double', $expression['value'], $expression);

			case 'bool':
				return new CompiledExpression('bool', $expression['value'], $expression);

			case 'string':
				if (ctype_digit($expression['value'])) {
					return new CompiledExpression('int', $expression['value'], $expression);
				}
				return new CompiledExpression('string', Utils::addSlashes($expression['value']), $expression);

			case 'char':
				if (strlen($expression['value']) > 2) {
					if (strlen($expression['value']) > 10) {
						throw new CompilerException("Invalid char literal: '" . substr($expression['value'], 0, 10) . "...'", $expression);
					} else {
						throw new CompilerException("Invalid char literal: '" . $expression['value'] . "'", $expression);
					}
				}
				return new CompiledExpression('char', $expression['value'], $expression);

			case 'variable':
				return new CompiledExpression('variable', $expression['value'], $expression);

			case 'constant':
				$constant = new Constants();
				$constant->setReadOnly($this->isReadOnly());
				$constant->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $constant->compile($expression, $compilationContext);

			case 'empty-array':
				return $this->emptyArray($expression, $compilationContext);

			case 'array-access':
				$arrayAccess = new NativeArrayAccess();
				$arrayAccess->setReadOnly($this->isReadOnly());
				$arrayAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $arrayAccess->compile($expression, $compilationContext);

			case 'property-access':
				$propertyAccess = new PropertyAccess();
				$propertyAccess->setReadOnly($this->isReadOnly());
				$propertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $propertyAccess->compile($expression, $compilationContext);

			case 'property-dynamic-access':
				$propertyAccess = new PropertyDynamicAccess();
				$propertyAccess->setReadOnly($this->isReadOnly());
				$propertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $propertyAccess->compile($expression, $compilationContext);

			case 'static-constant-access':
				$staticConstantAccess = new StaticConstantAccess();
				$staticConstantAccess->setReadOnly($this->isReadOnly());
				$staticConstantAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $staticConstantAccess->compile($expression, $compilationContext);

			case 'static-property-access':
				$staticPropertyAccess = new StaticPropertyAccess();
				$staticPropertyAccess->setReadOnly($this->isReadOnly());
				$staticPropertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $staticPropertyAccess->compile($expression, $compilationContext);

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
				$expr = new IssetOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'fetch':
				$expr = new FetchOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			case 'empty':
				$expr = new EmptyOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

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
					$compilationContext->logger->warning("Unnecessary extra parentheses", "extra-parentheses", $expression);
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

			case 'instanceof':
				return $this->compileInstanceOf($expression, $compilationContext);

			case 'clone':
				$expr = new CloneOperator();
				$expr->setReadOnly($this->isReadOnly());
				$expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
				return $expr->compile($expression, $compilationContext);

			/**
			 * @TODO implement this
			 */
			case 'unlikely':
			case 'likely':
			case 'ternary':
				return new CompiledExpression('int', '(0 == 1)', $expression);

			case 'typeof':
			case 'require':
				return new CompiledExpression('bool', '(0 == 1)', $expression);

			default:
				throw new CompilerException("Unknown expression: " . $type, $expression);
		}
	}

}
