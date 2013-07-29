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

		$condition = false;

		$exprRaw = $this->_statement['expr'];
		if (isset($exprRaw['type'])) {
			switch ($exprRaw['type']) {
				case 'identical':
				case 'equals':
				case 'not-identical':
				case 'not-equals':
					if ($exprRaw['left']['type'] == 'typeof' && $exprRaw['right']['type'] == 'string') {
						switch ($exprRaw['right']['type']) {
							case 'array':
								$condition = 'Z_TYPE_P(' . $exprRaw['left']['value'] . ') == IS_ARRAY';
								break;
							case 'string':
								$condition = 'Z_TYPE_P(' . $exprRaw['left']['left']['value'] . ') == IS_STRING';
								break;
						}
					}
					break;
			}
		}

		if ($condition) {
			$compilationContext->codePrinter->output('if (' . $condition . ') {');
		} else {

			$expr = new Expression($exprRaw);
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
				case 'isset':
					$compilationContext->codePrinter->output('//missing');
					break;
				case 'fcall':
					$compilationContext->codePrinter->output('//missing');
					break;
				default:
					throw new CompilerException("Expression can't be evaluated", $this->_statement['expr']);
			}
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
