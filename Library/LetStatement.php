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
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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
	 * Compiles foo = {expr}
	 */
	public function assignVariable($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
			ReadDetector $readDetector, CompilationContext $compilationContext, $statement)
	{

		if ($symbolVariable->isReadOnly())  {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

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
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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

						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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
								if ($itemVariable->getName() != $variable) {
									$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
								}
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
	 * Compiles foo[] = {expr}
	 */
	public function assignVariableAppend($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{
		if ($symbolVariable->isReadOnly())  {
			throw new CompilerException("Cannot write variable '" . $name. "' because it is read only", $statement);
		}

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
	 * Compiles foo[y] = {expr}
	 */
	public function assignArrayIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		//print_r($statement);

		$expression = new Expression($statement['index-expr']);
		$exprIndex = $expression->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'int':
			case 'string':
			case 'variable':
				break;
			default:
				throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array index in assigment without cast", $statement);
		}

		switch ($resolvedExpr->getType()) {
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableExpr->getType()) {
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						switch ($exprIndex->getType()) {
							case 'variable':
								$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $statement);
								switch ($variableIndex->getType()) {
									case 'int':
										$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
										break;
									case 'string':
										$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
										break;
									case 'variable':
										$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $resolvedExpr->getCode() . ', PH_COPY | PH_SEPARATE);');
										break;
									default:
										throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be assigned to array index", $statement);
								}
						}
						break;
					default:
						throw new CompilerException("Variable: " . $exprIndex->getType() . " cannot be assigned to array index", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array index", $statement);
		}

	}

	/**
	 * Compiles x++
	 */
	public function assignIncr($variable, Variable $symbolVariable, CompilationContext $compilationContext, $statement)
	{
		if ($symbolVariable->isReadOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

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

		if ($symbolVariable->isReadOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

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
				throw new CompilerException("Cannot decrement variable: " . $symbolVariable->getType(), $statement);
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
						/**
						 * @TODO Assign the real value : true or false
						 */
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
						$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
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

			$variable = $assignment['variable'];

			/**
			 * Get the symbol from the symbol table
			 */
			$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable, $assignment);

			/**
			 * Incr/Decr assignments don't require an expression
			 */
			if (isset($assignment['expr'])) {
				$readDetector = new ReadDetector($assignment['expr']);
				$expr = new Expression($assignment['expr']);
				switch ($assignment['assign-type']) {
					case 'variable':
						$expr->setExpectReturn(true, $symbolVariable);
						break;
				}
				$resolvedExpr = $expr->compile($compilationContext);
			}

			/**
			 * Variables assigned are marked as initialized (after expression)
			 */
			$symbolVariable->setIsInitialized(true);

			$codePrinter = $compilationContext->codePrinter;

			$codePrinter->outputBlankLine(true);

			/**
			 * There are four types of assignments
			 */
			switch ($assignment['assign-type']) {
				case 'variable':
					if (!isset($readDetector)) {
						print_r($assignment);
					}
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
				case 'object-property-array-index':
					//$this->assignArrayIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					$codePrinter->output('//missing object-property-array-index');
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