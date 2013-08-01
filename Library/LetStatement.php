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
 * LetStatement
 *
 * Let statement is used to assign variables
 */
class LetStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Compiles foo = x->y
	 */
	public function assignPropertyAccess($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$property = $statement['property'];

		switch ($resolvedExpr->getType()) {
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
				switch ($variableExpr->getType()) {
					case 'variable':
						/*$compilationContext->headersManager->add('kernel/array');
						switch ($variableIndex->getType()) {
							case 'int':
								$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
							case 'string':
								$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
							case 'variable':
								$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
						}*/

						break;
					default:
						throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be used as object", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array index", $statement);
		}

		/*$expr = new Expression($statement['expr']);
		$exprIndex = $expr->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode());
				switch ($variableIndex->getType()) {
					case 'int':
						break;
				}
		}

		print_r($statement);

		$codePrinter = $compilationContext->codePrinter;*/

	}

	/**
	 * Creates a new instance
	 *
	 */
	public function newInstance($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/fcall');

		$newExpr = $resolvedExpr->getCode();

		if (strtolower($newExpr['class']) == 'stdclass') {
			$codePrinter->output('object_init(' . $variable . ');');
		} else {
			if ($compilationContext->compiler->isClass($newExpr['class'])) {
				$classCe = strtolower(str_replace('\\', '_', $newExpr['class'])) . '_ce';
				$codePrinter->output('object_init_ex(' . $variable . ', ' . $classCe . ');');
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
			return;
		}

		/**
		 * Call the constructor
		 * For classes in the same extension we check if the class does implement a constructor
		 * For external classes we always assume the class does implement a constructor
		 */
		if ($compilationContext->compiler->isClass($newExpr['class'])) {
			$classDefinition = $compilationContext->compiler->getClassDefinition($newExpr['class']);
			if ($classDefinition->hasMethod("__construct")) {

				if (isset($newExpr['parameters'])) {
					$callExpr = new CompiledExpression('mcall', null, array(
						'variable' => $variable,
						'name' => '__construct',
						'parameters' => $newExpr['parameters']
					));
				} else {
					$callExpr = new CompiledExpression('mcall', null, array(
						'variable' => $variable,
						'name' => '__construct'
					));
				}

				$m = new MethodCall();
				$m->compile(null, null, $callExpr, $compilationContext, $statement);
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

	}

	/**
	 * Compiles foo = {expr}
	 */
	public function assignVariable($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
			ReadDetector $readDetector, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				switch ($resolvedExpr->getType()) {
					case 'null':
						$codePrinter->output($variable . ' = 0;');
						break;
					case 'int':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
						break;
					case 'double':
						$codePrinter->output($variable . ' = (long) (' . $resolvedExpr->getCode() . ');');
						break;
					case 'bool':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'bool':
								$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
								break;
							case 'double':
								$codePrinter->output($variable . ' = (long) ' . $itemVariable->getName() . ';');
								break;
							default:
								throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
						}
						break;
					case 'expr-variable':
						$codePrinter->output($variable . ' = zephir_get_intval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new CompilerException("Value type '" . $resolvedExpr->getType() . "' cannot be assigned to variable: int", $statement);
				}
				break;
			case 'double':
				switch ($resolvedExpr->getType()) {
					case 'null':
						$codePrinter->output($variable . ' = 0.0;');
						break;
					case 'int':
						$codePrinter->output($variable . ' = (double) (' . $resolvedExpr->getCode() . ');');
						break;
					case 'double':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
						break;
					case 'bool':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'bool':
								$codePrinter->output($variable . ' = (double) ' . $itemVariable->getName() . ';');
								break;
							case 'double':
								$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
								break;
							case 'variable':
								$codePrinter->output($variable . ' = zephir_get_doubleval(' . $itemVariable->getName() . ');');
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					case 'expr-variable':
						$codePrinter->output($variable . ' = zephir_get_doubleval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'string':
				switch ($resolvedExpr->getType()) {
					case 'null':
						$compilationContext->headersManager->add('kernel/string_type');
						$codePrinter->output('zephir_str_assign(' . $variable . ', "", sizeof("")-1));');
						break;
					case 'int':
						$codePrinter->output($variable . ' = (double) (' . $resolvedExpr->getCode() . ');');
						break;
					case 'string':
						$symbolVariable->setMustInitNull(true);
						$compilationContext->headersManager->add('kernel/string_type');
						$codePrinter->output('zephir_str_assign(' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1);');
						break;
					case 'double':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
						break;
					case 'bool':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($itemVariable->getType()) {
							/*case 'int':
								$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
								break;
							case 'double':
								$codePrinter->output($variable . ' = (long) ' . $itemVariable->getName() . ';');
								break;
							case 'variable':
								$codePrinter->output($variable . ' = zephir_get_doubleval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
								break;*/
							default:
								throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'bool':
				switch ($resolvedExpr->getType()) {
					case 'null':
						$codePrinter->output($variable . ' = 0;');
						break;
					case 'int':
					case 'double':
						$codePrinter->output($variable . ' = (' . $resolvedExpr->getCode() . ') ? 1 : 0;');
						break;
					case 'bool':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
						break;
					/*case 'variable':
						$codePrinter->output($variable . ' = zephir_get_boolval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;*/
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'double':
								$codePrinter->output($variable . ' = (' . $itemVariable->getName() . ') ? 1 : 0;');
								break;
							case 'bool':
								$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
								break;
							case 'variable':
								$codePrinter->output($variable . ' = zephir_get_boolval(' . $itemVariable->getName() . ');');
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					case 'expr-variable':
						$codePrinter->output($variable . ' = zephir_get_boolval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'double':
				switch ($resolvedExpr->getType()) {
					case 'int':
						$codePrinterrinter->output($variable . ' = (double) (' . $resolvedExpr->getCode() . ');');
						break;
					case 'double':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
						break;
					case 'variable':
						$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'variable':

				switch ($resolvedExpr->getType()) {
					case 'null':
						if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
							$codePrinter->output('ZVAL_NULL(' . $variable . ');');
						} else {
							$symbolVariable->initVariant($compilationContext);
							$codePrinter->output('ZVAL_NULL(' . $variable . ');');
						}
						break;
					case 'int':
						if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
							$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						} else {
							$symbolVariable->initVariant($compilationContext);
							$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						}
						break;
					case 'double':
						if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
							$codePrinter->output('ZVAL_DOUBLE(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						} else {
							$symbolVariable->initVariant($compilationContext);
							$codePrinter->output('ZVAL_DOUBLE(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						}
						break;
					case 'bool':
						$symbolVariable->initVariant($compilationContext);
						if ($resolvedExpr->getCode() == 'true') {
							$codePrinter->output('ZVAL_BOOL(' . $variable . ', 1);');
						} else {
							if ($resolvedExpr->getCode() == 'false') {
								$codePrinter->output('ZVAL_BOOL(' . $variable . ', 0);');
							} else {
								$codePrinter->output('ZVAL_BOOL(' . $variable . ', ' . $resolvedExpr->getBooleanCode() . ');');
							}
						}
						break;
					case 'string':
						$symbolVariable->initVariant($compilationContext);
						$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
						break;
					case 'variable':

						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($itemVariable->getType()) {
							case 'int':
								$symbolVariable->initVariant($compilationContext);
								$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $itemVariable->getName() . ');');
								break;
							case 'double':
								$symbolVariable->initVariant($compilationContext);
								$codePrinter->output('ZVAL_DOUBLE(' . $variable . ', ' . $itemVariable->getName() . ');');
								break;
							case 'bool':
								$symbolVariable->initVariant($compilationContext);
								$codePrinter->output('ZVAL_BOOL(' . $variable . ', ' . $itemVariable->getName() . ');');
								break;
							case 'variable':
								$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					case 'expr-variable':
						if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
							$code = $resolvedExpr->resolve(null, $compilationContext);
							$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $code . ');');
						} else {
							$symbolVariable->initVariant($compilationContext);
							$codePrinter->output($resolvedExpr->resolve($variable, $compilationContext));
						}
						break;
					case 'empty-array':
						$symbolVariable->initVariant($compilationContext);
						$codePrinter->output('array_init(' . $variable . ');');
						break;
					case 'array':
						$symbolVariable->initVariant($compilationContext);
						$this->assignArray($variable, $resolvedExpr, $compilationContext, $statement);
						break;
					case 'array-access':
						$this->arrayAccess($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
						break;
					case 'property-access':
						$this->propertyAccess($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
						break;
					case 'new-instance':
						$this->newInstance($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
						break;
					case 'fcall':
						//$this->newInstance($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
						$codePrinter->output('//missing fcall');
						break;
					case 'mcall':
						$methodCall = new MethodCall();
						$methodCall->compile($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
						break;
					default:
						throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type: $type", $statement);
		}
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
				$itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $item);
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
	public function assignArray($variable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$codePrinter->output('array_init(' . $variable . ');');
		foreach ($resolvedExpr->getCode() as $item) {
			if (isset($item['key'])) {
				$key = null;
				switch ($item['key']['type']) {
					case 'string':
						switch ($item['value']['type']) {
							case 'int':
								$codePrinter->output('add_assoc_long_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), ' . $item['value']['value'] . ');');
								break;
							case 'double':
								$codePrinter->output('add_assoc_double_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), ' . $item['value']['value'] . ');');
								break;
							case 'bool':
								if ($item['value']['value'] == 'true') {
									$codePrinter->output('add_assoc_bool_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), 1);');
								} else {
									$codePrinter->output('add_assoc_bool_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), 0);');
								}
								break;
							case 'string':
								$codePrinter->output('add_assoc_stringl_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), SL("' . $item['value']['value'] . '"), 1);');
								break;
							case 'null':
								$codePrinter->output('add_assoc_null_ex(' . $variable . ', SS("' . $item['key']['value'] . '"));');
								break;
							default:
								throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
						}
						break;
					case 'int':
						switch ($item['value']['type']) {
							case 'int':
								$codePrinter->output('add_index_double(' . $variable . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
								break;
							case 'bool':
								if ($item['value']['value'] == 'true') {
									$codePrinter->output('add_index_bool(' . $variable . ', ' . $item['key']['value'] . ', 1);');
								} else {
									$codePrinter->output('add_index_bool(' . $variable . ', ' . $item['key']['value'] . ', 0);');
								}
								break;
							case 'double':
								$codePrinter->output('add_index_double(' . $variable . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
								break;
							case 'null':
								$codePrinter->output('add_index_null(' . $variable . ', ' . $item['key']['value'] . ');');
								break;
							case 'string':
								$codePrinter->output('add_index_stringl(' . $variable . ', ' . $item['key']['value'] . ', SL("' . $item['value']['value'] . '"), 1);');
								break;
							case 'variable':
								$value = $this->getArrayValue($item, $compilationContext);
								$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $item['key']['value'] . ', &' . $value . ', PH_COPY | PH_SEPARATE);');
								break;
							default:
								throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
						}
						break;
					case 'variable':
						$variableVariable = $compilationContext->symbolTable->getVariableForRead($item['key']['value']);
						switch ($variableVariable->getType()) {
							case 'int':
								switch ($item['value']['type']) {
									case 'int':
										$codePrinter->output('add_index_double(' . $variable . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
										break;
									case 'bool':
										if ($item['value']['value'] == 'true') {
											$codePrinter->output('add_index_bool(' . $variable . ', ' . $item['key']['value'] . ', 1);');
										} else {
											$codePrinter->output('add_index_bool(' . $variable . ', ' . $item['key']['value'] . ', 0);');
										}
										break;
									case 'double':
										$codePrinter->output('add_index_double(' . $variable . ', ' . $item['key']['value'] . ', ' . $item['value']['value'] . ');');
										break;
									case 'null':
										$codePrinter->output('add_index_null(' . $variable . ', ' . $item['key']['value'] . ');');
										break;
									case 'string':
										$codePrinter->output('add_index_stringl(' . $variable . ', ' . $item['key']['value'] . ', SL("' . $item['value']['value'] . '"), 1);');
										break;
									case 'variable':
										$value = $this->getArrayValue($item, $compilationContext);
										$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $item['key']['value'] . ', &' . $value . ', PH_COPY | PH_SEPARATE);');
										break;
									default:
										throw new CompilerException("Invalid value type: " . $item['value']['type'], $item['value']);
								}
								break;
							case 'string':
								switch ($item['value']['type']) {
									case 'int':
										$codePrinter->output('add_assoc_long_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $item['value']['value'] . ');');
										break;
									case 'double':
										$codePrinter->output('add_assoc_double_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, ' . $item['value']['value'] . ');');
										break;
									case 'bool':
										if ($item['value']['value'] == 'true') {
											$codePrinter->output('add_assoc_bool_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, 1);');
										} else {
											$codePrinter->output('add_assoc_bool_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len, 0);');
										}
										break;
									case 'string':
										$codePrinter->output('add_assoc_stringl_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1, SL("' . $item['value']['value'] . '"), 1);');
										break;
									case 'null':
										$codePrinter->output('add_assoc_null_ex(' . $variable . ', ' . $item['key']['value'] . '->str, ' . $item['key']['value'] . '->len + 1);');
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
				$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $item . ', 0);');
			}
		}
	}

	/**
	 * Compiles foo[] = {expr}
	 */
	public function assignVariableAppend($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				throw new CompilerException("Cannot append to 'int' variables", $statement);
			case 'variable':
				switch ($resolvedExpr->getType()) {
					case 'variable':
						 $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $resolvedExpr->getCode() . ', PH_SEPARATE);');
						 break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type", $statement);
		}
	}

	/**
	 * Compiles foo->x = {expr}
	 */
	public function propertyAccess($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{
		$codePrinter = $compilationContext->codePrinter;

		$propertyAccess = $resolvedExpr->getOriginal();

		$expr = new Expression($propertyAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $statement);
				switch ($variableVariable->getType()) {
					case 'variable':
						break;
					default:
						throw new CompiledException("Variable type: " . $variableVariable->getType() . " cannot be used as object", $propertyAccess['left']);
				}
				break;
			default:
				throw new CompiledException("Cannot use expression: ". $exprVariable->getType() . " as an object", $propertyAccess['left']);
		}

		$property = $propertyAccess['right']['value'];

		$compilationContext->headersManager->add('kernel/object');
		$symbolVariable->observeVariant($compilationContext);

		if ($variableVariable->getName() == 'this') {
			//zephir_read_property_this?
			$codePrinter->output('zephir_read_property(&' . $variable . ', this_ptr, SL("' . $property . '"), PH_NOISY_CC);');
		} else {
			$codePrinter->output('zephir_read_property(&' . $variable . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
		}

	}

	/**
	 * Compiles foo[x] = {expr}
	 */
	public function arrayAccess($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$symbolVariable->observeVariant($compilationContext);

		$arrayAccess = $resolvedExpr->getOriginal();

		$expr = new Expression($arrayAccess['left']);
		$exprVariable = $expr->compile($compilationContext);

		switch ($exprVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $statement);
				switch ($variableVariable->getType()) {
					case 'variable':
						break;
					default:
						throw new CompiledException("Variable type: " . $variableVariable->getType() . " cannot be used as array", $arrayAccess['left']);
				}
				break;
			default:
				throw new CompiledException("Cannot use expression: ". $exprVariable->getType() . " as an array", $arrayAccess['left']);
		}

		$expr = new Expression($arrayAccess['right']);
		$exprIndex = $expr->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'int':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_long(&' . $variable . ', ' . $variableVariable->getName() . ', ' . $exprIndex->getCode() . ', PH_NOISY);');
				break;
			case 'string':
				$compilationContext->headersManager->add('kernel/array');
				$codePrinter->output('zephir_array_fetch_string(&' . $variable . ', ' . $variableVariable->getName() . ', SL("' . $exprIndex->getCode() . '"), PH_NOISY);');
				break;
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $statement);
				switch ($variableIndex->getType()) {
					case 'int':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch_long(&' . $variable . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', PH_NOISY);');
						break;
					case 'string':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch_string(&' . $variable . ', ' . $variableVariable->getName() . ', ' . $exprIndex->getCode() . '->str, ' . $exprIndex->getCode() . '->len, PH_NOISY);');
						break;
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						$codePrinter->output('zephir_array_fetch(&' . $variable . ', ' . $variableVariable->getName() . ', ' . $variableIndex->getName() . ', PH_NOISY);');
						break;
					default:
						throw new CompiledException("Variable type: " . $exprIndex->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
				}
				break;
			default:
				throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $arrayAccess['right']);
		}

	}

	/**
	 * Compiles foo[] = expr
	 */
	public function assignArrayIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{
		//PHALCON_OBS_VAR(url);
		//phalcon_array_fetch_string(&url, _GET, SL("_url"), PH_NOISY);

		//phalcon_array_update_zval(&parts, part, &converted_part, PH_COPY | PH_SEPARATE);

		$codePrinter = $compilationContext->codePrinter;

		$variableIndex = $compilationContext->symbolTable->getVariableForRead($statement['property'], $statement);
		switch ($variableIndex->getType()) {
			case 'int':
			case 'string':
			case 'variable':
				break;
			default:
				throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index in assigment without cast", $statement);
		}

		switch ($resolvedExpr->getType()) {
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
				switch ($variableExpr->getType()) {
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						switch ($variableIndex->getType()) {
							case 'int':
								$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
							case 'string':
								$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
							case 'variable':
								$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $statement['property'] . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
								break;
						}
						break;
					default:
						throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be assigned to array index", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array index", $statement);
		}

		/*$expr = new Expression($statement['expr']);
		$exprIndex = $expr->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode());
				switch ($variableIndex->getType()) {
					case 'int':
						break;
				}
		}

		print_r($statement);

		$codePrinter = $compilationContext->codePrinter;*/

	}

	/**
	 * Compiles x++
	 */
	public function assignIncr($variable, Variable $symbolVariable, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		switch ($symbolVariable->getType()) {
			case 'int':
				$codePrinter->output($variable . '++;');
				break;
			case 'double':
				$codePrinter->output($variable . '++;');
				break;
			default:
				throw new CompilerException("Cannot increment: " . $symbolVariable->getType(), $statement);
		}
	}

	/**
	 * Compiles x--
	 */
	public function assignDecr($variable, Variable $symbolVariable, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		switch ($symbolVariable->getType()) {
			case 'int':
				$codePrinter->output($variable . '--;');
				break;
			case 'double':
				$codePrinter->output($variable . '--;');
				break;
			case 'variable':
				$compilationContext->headersManager->add('kernel/operators');
				$symbolVariable->initVariant($compilationContext);
				$codePrinter->output('zephir_decrement(' . $variable . ');');
				break;
			default:
				throw new CompilerException("Cannot decrement: " . $symbolVariable->getType(), $statement);
		}
	}

	/**
	 * Compiles foo[] = {expr}
	 */
	public function assignObjectProperty($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * @TODO use zephir_update_property_this
		 */
		if ($variable == 'this') {
			$variable = 'this_ptr';
		}

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				throw new CompilerException("Variable 'int' cannot be used as object", $statement);
			case 'bool':
				throw new CompilerException("Variable 'bool' cannot be used as object", $statement);
			case 'variable':
				$propertyName = $statement['property'];
				$compilationContext->headersManager->add('kernel/object');
				$symbolVariable->initVariant($compilationContext);
				switch ($resolvedExpr->getType()) {
					case 'bool':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('zephir_update_property_zval(' . $variable . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'null':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('zephir_update_property_zval(' . $variable . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'empty-array':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('array_init(' . $tempVariable->getName() . ');');
						$codePrinter->output('zephir_update_property_zval(' . $variable . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'variable':
						$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $statement);
						switch ($variableVariable->getType()) {
							case 'variable':
								if ($variable == 'this') {
									if (!$compilationContext->classDefinition->hasProperty($propertyName)) {
										throw new CompilerException("Property '" . $propertyName . "' is not defined on class '" . $propertyName . "'", $statement);
									}
									$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
								} else {
									$codePrinter->output('zephir_update_property_zval(' . $variable . ', SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
								}
								break;
							default:
								throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type", $statement);
		}
	}

	/**
	 * Compiles the let statement
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$statement = $this->_statement;

		foreach ($statement['assignments'] as $assignment) {

			/**
			 * Incr/Decr assigments don't require an expression
			 */
			if (isset($assignment['expr'])) {
				$readDetector = new ReadDetector($assignment['expr']);
				$expr = new Expression($assignment['expr']);
				$resolvedExpr = $expr->compile($compilationContext);
			}

			$variable = $assignment['variable'];

			/**
			 * Get the symbol from the symbol table
			 */
			$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable, $assignment);

			/**
			 * Variables assigned are marked as initialized
			 */
			$symbolVariable->setIsInitialized(true);

			$codePrinter = $compilationContext->codePrinter;

			$codePrinter->outputBlankLine(true);

			/**
			 * There are four types of assignments
			 */
			switch ($assignment['assign-type']) {
				case 'variable':
					$this->assignVariable($variable, $symbolVariable, $resolvedExpr, $readDetector, $compilationContext, $assignment);
					break;
				case 'variable-append':
					$this->assignVariableAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property':
					$this->assignObjectProperty($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'array-index':
					$this->assignArrayIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'property-access':
					$this->assignPropertyAccess($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property-append':
					//$this->assignArrayIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					$codePrinter->output('//missing object-property-append');
					break;
				case 'incr':
					$this->assignIncr($variable, $symbolVariable, $compilationContext, $assignment);
					break;
				case 'decr':
					$this->assignDecr($variable, $symbolVariable, $compilationContext, $assignment);
					break;
				default:
					throw new CompilerException("Unknown assignment: " . $assignment['assign-type'], $assignment);
			}

			$codePrinter->outputBlankLine(true);
		}
	}

}