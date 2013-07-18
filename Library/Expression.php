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
			case 'int':
				return $this->_expression;
			case 'string':
				return $this->_expression;
			default:
				throw new Exception("Unknown " . $type);
		}
		echo $type;
	}

	public function compileEquals($expression, SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		//echo '[', $expression['left']['type'], ']', PHP_EOL;

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($symbolTable, $classDefinition);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($symbolTable, $classDefinition);

		switch ($left->getType()) {
			case 'variable':

				$variable = $symbolTable->getVariableForRead($expression['left']['value']);

				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')');
					default:
						throw new Exception("Error Processing Request");
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' == ' . $right->getCode());
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' == (int) ' . $right->getCode());
					default:
						throw new Exception("Error Processing Request");
				}
				break;
		}


	}

	/**
	 * Resolves an expression
	 *
	 */
	public function compile(SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{

		$expression = $this->_expression;

		$type = $expression['type'];
		switch ($type) {
			case 'int':
				return new CompiledExpression('int', $this->_expression['value']);
			case 'string':
				return new CompiledExpression('string', $this->_expression['value']);
			case 'variable':
				return new CompiledExpression('variable', $this->_expression['value']);
			case 'equals':
				return $this->compileEquals($expression, $symbolTable, $classDefinition);
			default:
				throw new Exception("Unknown " . $type . " " . print_r($expression, true));
		}
		//echo $type;
	}

}