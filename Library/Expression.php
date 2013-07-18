<?php

class Expression
{

	protected $_expression;

	public function __construct(array $expression)
	{
		$this->_expression = $expression;
	}

	/**
	 * Resolves an expression
	 *
	 */
	public function resolve(SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		$type = $this->_expression['type'];
		switch ($type) {
			case 301:
				return $this->_expression;
				break;
			case 303:
				return $this->_expression;
				break;
			default:
				throw new Exception("Unknown " . $type);
		}
		echo $type;
	}

	/**
	 * Resolves an expression
	 *
	 */
	public function compile(SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{

		$expression = $this->_expression;

		if (isset($expression['left'])) {
			$leftExpr = new Expression($expression['left']);
			$left = $leftExpr->compile($symbolTable, $classDefinition);
		}

		if (isset($expression['right'])) {
			$rightExpr = new Expression($expression['right']);
			$right = $rightExpr->compile($symbolTable, $classDefinition);
		}

		$type = $expression['type'];
		switch ($type) {
			case 301:
				return $this->_expression['value'];
			case 303:
				return $this->_expression['value'];
			case 307:
				return $this->_expression['value'];
			case 400:
				return $left . ' == ' . $right;
			default:
				throw new Exception("Unknown " . $type . " " . print_r($expression, true));
		}
		//echo $type;
	}

}