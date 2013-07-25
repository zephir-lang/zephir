<?php

/**
 * WhileStatement
 *
 * While statement, the same as in PHP/C
 */
class WhileStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Perform the compilation of code
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
				$compilationContext->codePrinter->output('while (' . $compiledExpression->getCode() . ') {');
				break;
			case 'bool':
				$compilationContext->codePrinter->output('while (' . $compiledExpression->getBooleanCode() . ') {');
				break;
			case 'variable':

				$variableRight = $compilationContext->symbolTable->getVariableForRead($this->_statement['expr']['value']);
				switch ($variableRight->getType()) {
					case 'int':
						$compilationContext->codePrinter->output('while (' . $variableRight->getName() . ') {');
						break;
					case 'bool':
						$compilationContext->codePrinter->output('while (' . $variableRight->getName() . ') {');
						break;
					case 'double':
						$compilationContext->codePrinter->output('while (' . $variableRight->getName() . ') {');
						break;
					case 'variable':
						$compilationContext->codePrinter->output('while (zend_is_true(' . $variableRight->getName() . ')) {');
						break;
					default:
						throw new CompilerException("Variable " . $variableRight->getType() . " can't be evaluated", $this->_statement);
				}
				break;
			default:
				throw new CompilerException("Expression can't be evaluated", $this->_statement);
		}

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		/**
		 * Compile statements in the 'if' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Restore the cycle counter
		 */
		$compilationContext->insideCycle--;

		$compilationContext->codePrinter->output('}');

	}

}
