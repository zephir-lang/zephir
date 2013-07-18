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

	public function compile(CompilationContext $compilationContext)
	{
		$expr = new Expression($this->_statement['expr']);

		$variable = $this->_statement['variable'];

		$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable);

		$symbolVariable->setInitialized(true);

		$resolvedExpr = $expr->resolve($compilationContext);

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				switch ($resolvedExpr['type']) {
					case 'int':
						$compilationContext->codePrinter->output($variable . ' = ' . $resolvedExpr['value'] . ';');
						break;
				}
				break;
			case 'var':
				$symbolVariable->initVariant($codePrinter);
				switch ($resolvedExpr['type']) {
					case 'int':
						$compilationContext->codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr['value'] . ');');
						break;
					case 'string':
						$compilationContext->codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr['value'] . '", 1);');
						break;
				}
				break;
		}

		//echo $resolvedExpr['type'];

		//echo $type;
	}
}