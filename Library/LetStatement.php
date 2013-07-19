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
	public function assignVariable($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				switch ($resolvedExpr['type']) {
					case 'int':
						$codePrinter->output($variable . ' = ' . $resolvedExpr['value'] . ';');
						break;
					default:
						throw new Exception("Unknown type");
				}
				break;
			case 'variable':
				$symbolVariable->initVariant($compilationContext);
				switch ($resolvedExpr->getType()) {
					case 'int':
						$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
						break;
					case 'string':
						$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
						break;
					case 'empty-array':
						$codePrinter->output('array_init(' . $variable . ');');
						break;
					case 'array':
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
				throw new Exception("Unknown type");
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

		$expr = new Expression($statement['expr']);

		$variable = $statement['variable'];

		//print_r($this->_statement);

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
		switch ($this->_statement['assign-type']) {
			case 'variable':
				$this->assignVariable($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
				break;
			case 'variable-append':
				$this->assignVariableAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
				break;
			case 'object-property':
				$this->assignObjectProperty($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
				break;
			default:
				throw new Exception("Unknown assignment: " . $this->_statement['assign-type']);
		}

		$codePrinter->outputBlankLine(true);
	}
}