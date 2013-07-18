<?php

/**
 * Expressions
 *
 * Represents an expression
 * Most in a language is an expression
 */
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
	public function resolve(CompilationContext $compilationContext)
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
	}

	public function compileEquals($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		//echo '[', $expression['left']['type'], ']', PHP_EOL;

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

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
	public function compile(CompilationContext $compilationContext)
	{

		$expression = $this->_expression;

		$type = $expression['type'];
		switch ($type) {
			case 'int':
				return new CompiledExpression('int', $expression['value']);
			case 'string':
				return new CompiledExpression('string', $expression['value']);
			case 'variable':
				return new CompiledExpression('variable', $expression['value']);
			case 'equals':
				return $this->compileEquals($expression, $compilationContext);
			default:
				throw new Exception("Unknown " . $type . " " . print_r($expression, true));
		}
		//echo $type;
	}

}