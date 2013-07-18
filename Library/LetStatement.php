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

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		$expr = new Expression($this->_statement['expr']);

		$variable = $this->_statement['variable'];

		$symbolVariable = $symbolTable->getVariableForWrite($variable);

		$symbolVariable->setInitialized(true);

		$resolvedExpr = $expr->resolve($symbolTable, $classDefinition);

		$type = $symbolVariable->getType();
		switch ($type) {
			case VAR_TYPE_INT:
				switch ($resolvedExpr['type']) {
					case EXPR_INT:
						$codePrinter->output($variable . ' = ' . $resolvedExpr['value'] . ';');
						break;
				}
				break;
			case VAR_TYPE_VAR:
				$symbolVariable->initVariant($codePrinter);
				switch ($resolvedExpr['type']) {
					case EXPR_INT:
						$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr['value'] . ');');
						break;
					case EXPR_STRING:
						$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr['value'] . '", 1);');
						break;
				}
				break;
		}

		echo $resolvedExpr['type'];

		//echo $type;
	}
}