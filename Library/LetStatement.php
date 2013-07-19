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
	public function assignVariable($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext)
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

	public function compile(CompilationContext $compilationContext)
	{
		$expr = new Expression($this->_statement['expr']);

		$variable = $this->_statement['variable'];

		$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable);

		/**
		 * Variables assigned are marked as initialized
		 */
		$symbolVariable->setIsInitialized(true);

		$resolvedExpr = $expr->compile($compilationContext);

		$codePrinter = $compilationContext->codePrinter;

		$codePrinter->output('');

		/**
		 * There are four types of assignments
		 */
		switch ($this->_statement['assign-type']) {
			case 'variable':
				$this->assignVariable($variable, $symbolVariable, $resolvedExpr, $compilationContext);
				break;
			case 'variable-append':
				$this->assignVariableAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext);
				break;
			default:
				throw new Exception("Unknown assignment: " . $this->_statement['assign-type']);

		}

		$codePrinter->output('');

		//echo $resolvedExpr['type'];

		//echo $type;
	}
}