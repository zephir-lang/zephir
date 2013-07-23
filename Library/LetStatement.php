<?php

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
	 * Compiles foo = expr
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
						$codePrinter->output($variable . ' = (int) (' . $resolvedExpr->getCode() . ');');
						break;
					case 'bool':
						$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
						break;
					case 'variable':
						$codePrinter->output($variable . ' = zephir_get_intval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
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
						$codePrinter->output($variable . ' = zephir_get_doubleval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
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
					case 'variable':
						$codePrinter->output($variable . ' = zephir_get_boolval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
						break;
					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
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
						throw new Exception("Unknown type " . $resolvedExpr->getType());
				}
				break;
			case 'variable':

				switch ($resolvedExpr->getType()) {
					case 'null':
						$codePrinter->output('ZVAL_NULL(' . $variable . ');');
						break;
					case 'int':
						$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						break;
					case 'double':
						//print_r($resolvedExpr->getOriginal());
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
								$codePrinter->output('ZVAL_BOOL(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
							}
						}
						break;
					case 'string':
						$symbolVariable->initVariant($compilationContext);
						$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
						break;
					case 'variable':
						if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
							$code = $resolvedExpr->resolve(null, $compilationContext);
							$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $code . ');');
						} else {
							$codePrinter->output($resolvedExpr->resolve($variable, $compilationContext));
						}
						break;
					case 'empty-array':
						$symbolVariable->initVariant($compilationContext);
						$codePrinter->output('array_init(' . $variable . ');');
						break;
					case 'array':
						$symbolVariable->initVariant($compilationContext);
						$codePrinter->output('array_init(' . $variable . ');');
						foreach ($resolvedExpr->getCode() as $item) {
							if (isset($item['key'])) {
								$key = null;
								switch ($item['key']['type']) {
									case 'string':
										$codePrinter->output('add_assoc_long_ex(' . $variable . ', SS("' . $item['key']['value'] . '"), 1);');
										break;
									case 'int':
										break;
								}
							} else {
								switch ($item['value']['type']) {
									case 'int':
										$compilationContext->headersManager->add('kernel/array');
										$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
										$codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $item['value']['value'] . ');');
										$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $tempVar->getName() . ', 0);');
										break;
									case 'double':
										$compilationContext->headersManager->add('kernel/array');
										$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
										$codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $item['value']['value'] . ');');
										$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $tempVar->getName() . ', 0);');
										break;
									case 'bool':
										$compilationContext->headersManager->add('kernel/array');
										$tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
										if ($item['value']['value'] == 'true') {
											$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', 1);');
										} else {
											$codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', 0);');
										}
										$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $tempVar->getName() . ', 0);');
										break;
									default:
										throw new Exception("Unknown");
								}
							}
						}
						break;
					case 'new-instance':

						$newExpr = $resolvedExpr->getCode();

						$classCe = strtolower(str_replace('\\', '_', $newExpr['class'])) . '_ce';

						$codePrinter->output('object_init_ex(' . $variable . ', ' . $classCe . ');');

						$params = array();
						foreach ($newExpr['parameters'] as $parameter) {
							$expr = new Expression($parameter);
							$compiledExpression = $expr->compile($compilationContext);
							$params[] = $compiledExpression->getCode();
						}

						$codePrinter->output('zephir_call_method_p' . count($params) . '_noret(' . $variable . ', "__construct", ' . join(', ', $params) . ');');
						break;

					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
				}
				break;
			default:
				throw new Exception("Unknown type $type");
		}
	}

	/**
	 * Compiles foo[] = expr
	 */
	public function assignVariableAppend($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				throw new Exception("Cannot append to 'int' variables");
			case 'variable':
				$symbolVariable->initVariant($compilationContext);
				switch ($resolvedExpr->getType()) {
					case 'variable':
						 $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $resolvedExpr->getCode() . ', PH_SEPARATE);');
						 break;
					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
				}
				break;
			default:
				throw new Exception("Unknown type");
		}
	}

	/**
	 * Compiles foo[] = expr
	 */
	public function assignObjectProperty($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				throw new Exception("Variable 'int' cannot be used as object");
			case 'bool':
				throw new Exception("Variable 'bool' cannot be used as object");
			case 'variable':
				$symbolVariable->initVariant($compilationContext);
				switch ($resolvedExpr->getType()) {
					case 'variable':

						$compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode());
						$propertyName = $statement['property'];

						if (!$compilationContext->classDefinition->hasProperty($propertyName)) {
							throw new Exception("Property '" . $propertyName . "' is not defined on class '" . $propertyName . "'");
						}

						$compilationContext->headersManager->add('kernel/object');

						if ($variable == 'this') {
							$codePrinter->output('phalcon_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
						}

						break;
					default:
						throw new Exception("Unknown type " . $resolvedExpr->getType());
				}
				break;
			default:
				throw new Exception("Unknown type");
		}
	}

	public function compile(CompilationContext $compilationContext)
	{
		$statement = $this->_statement;

		foreach ($statement['assignments'] as $assignment) {

			$readDetector = new ReadDetector($assignment['expr']);

			$expr = new Expression($assignment['expr']);

			$variable = $assignment['variable'];

			$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable);

			/**
			 * Variables assigned are marked as initialized
			 */
			$symbolVariable->setIsInitialized(true);

			$resolvedExpr = $expr->compile($compilationContext);

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
				default:
					throw new Exception("Unknown assignment: " . $assignment['assign-type']);
			}

			$codePrinter->outputBlankLine(true);
		}
	}
}