<?php

/**
 * ReturnStatement
 *
 * Return statement is used to assign variables
 */
class ReturnStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CompilationContext $compilationContext)
	{

		$statement = $this->_statement;

		$expr = new Expression($statement['expr']);

		$resolvedExpr = $expr->compile($compilationContext);

		$codePrinter = $compilationContext->codePrinter;
		$codePrinter->outputBlankLine(true);

		switch ($resolvedExpr->getType()) {
			case 'int':
				$codePrinter->output('RETURN_MM_LONG(' . $resolvedExpr->getCode() . ');');
				break;
			case 'double':
				$codePrinter->output('RETURN_MM_DOUBLE(' . $resolvedExpr->getCode() . ');');
				break;
			case 'variable':
				$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode());
				switch ($symbolVariable->getType()) {
					case 'int':
						$codePrinter->output('RETURN_MM_LONG(' . $symbolVariable->getName() . ');');
						break;
					case 'double':
						$codePrinter->output('RETURN_MM_DOUBLE(' . $symbolVariable->getName() . ');');
						break;
					case 'bool':
						$codePrinter->output('RETURN_MM_BOOL(' . $symbolVariable->getName() . ');');
						break;
					case 'variable':
						$codePrinter->output('RETURN_CCTOR(' . $symbolVariable->getName() . ');');
						break;
					default:
						throw new Exception("Cannot return variable '" . $symbolVariable->getType() . "'");
				}
				break;
			default:
				throw new Exception("Cannot return variable '" . $resolvedExpr->getType() . "'");
		}

		//print_r($variable);

		$codePrinter->outputBlankLine(true);
	}

}