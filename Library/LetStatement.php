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

		/**
		 * Variables assigned are marked as initialized
		 */
		$symbolVariable->setInitialized(true);

		$resolvedExpr = $expr->resolve($compilationContext);

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
				switch ($resolvedExpr['type']) {
					case 'int':
						$compilationContext->codePrinter->output($variable . ' = ' . $resolvedExpr['value'] . ';');
						break;
					default:
						throw new Exception("Unknown type");
				}
				break;
			case 'variable':
				$symbolVariable->initVariant($compilationContext->codePrinter);
				switch ($resolvedExpr['type']) {
					case 'int':
						$compilationContext->codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $resolvedExpr['value'] . ');');
						break;
					case 'string':
						$compilationContext->codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr['value'] . '", 1);');
						break;
					default:
						throw new Exception("Unknown type");
				}
				break;
			default:
				throw new Exception("Unknown type");
		}

		//echo $resolvedExpr['type'];

		//echo $type;
	}
}