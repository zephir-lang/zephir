<?php

class EchoStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CompilationContext $compilationContext)
	{
		$expr = $this->_statement['expr'];
		switch ($expr['type']) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expr['value']);

				switch ($variable->getType()) {
					case 'int':
						$compilationContext->codePrinter->output('fprintf(stdout, "%d", ' . $expr['value'] . ');');
						break;
					case 'variable':
						$compilationContext->codePrinter->output('zval_print(' . $expr['value'] . ');');
						break;
					default:
						throw new Exception("Unknown type: " . $variable->getType());
				}
		}
	}

}