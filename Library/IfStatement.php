<?php

/**
 * IfStatement
 *
 * If statement, the same as in PHP/C
 */
class IfStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		$expr = new Expression($this->_statement['expr']);

		$compiledExpression = $expr->compile($symbolTable, $classDefinition);

		$codePrinter->output('if (' . $compiledExpression->getCode() . ') {');
		$codePrinter->output('}');
	}
}