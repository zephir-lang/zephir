<?php

class EchoStatement
{
	public function __construct($statement)
	{

	}

	public function compile()
	{
		return 'zval_print();' . PHP_EOL;
	}
}