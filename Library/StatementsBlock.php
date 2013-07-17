<?php

class StatementsBlock
{
	protected $_statements;

	public function __construct(array $statements)
	{	
		$this->_statements = $statements;
	}	

	public function compile()
	{
		$code = '';
		foreach ($this->_statements as $statement) {
			switch ($statement['type']) {
				case 'echo':
					$echoStatement = new EchoStatement($statement);
					$code .= $echoStatement->compile();
					break;
			}
		}
		return $code;
	}
}