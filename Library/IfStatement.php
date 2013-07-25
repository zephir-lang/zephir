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

	/**
	 *
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$expr = new Expression($this->_statement['expr']);

		$compiledExpression = $expr->compile($compilationContext);

		/**
		 * Generate the condition according to the value returned by the evaluted expression
		 */
		switch ($compiledExpression->getType()) {
			case 'int':
			case 'double':
				$compilationContext->codePrinter->output('if (' . $compiledExpression->getCode() . ') {');
				break;
			case 'bool':
				$compilationContext->codePrinter->output('if (' . $compiledExpression->getBooleanCode() . ') {');
				break;
			case 'variable':

				$variableRight = $compilationContext->symbolTable->getVariableForRead($this->_statement['expr']['value'], $this->_statement['expr']);
				switch ($variableRight->getType()) {
					case 'int':
						$compilationContext->codePrinter->output('if (' . $variableRight->getName() . ') {');
						break;
					case 'bool':
						$compilationContext->codePrinter->output('if (' . $variableRight->getName() . ') {');
						break;
					case 'double':
						$compilationContext->codePrinter->output('if (' . $variableRight->getName() . ') {');
						break;
					case 'variable':
						$compilationContext->codePrinter->output('if (zend_is_true(' . $variableRight->getName() . ')) {');
						break;
					default:
						throw new CompiledException("Variable can't be evaluated " . $variableRight->getType(), $this->_statement['expr']);
				}
				break;
			default:
				throw new CompiledException("Expression can't be evaluated", $this->_statement['expr']);
		}

		/**
		 * Compile statements in the 'if' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Compile statements in the 'else' block
		 */
		if (isset($this->_statement['else_statements'])) {
			$compilationContext->codePrinter->output('} else {');
			$st = new StatementsBlock($this->_statement['else_statements']);
			$st->compile($compilationContext);
		}

		$compilationContext->codePrinter->output('}');

	}

}
