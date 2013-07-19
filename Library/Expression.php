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

	private function _addSlaches($str)
	{
		$str = str_replace('\\', "\\\\", $str);
		$str = str_replace('"', "\\\"", $str);
		$str = str_replace("\n", "\\n", $str);
		$str = str_replace("\r", "\\r", $str);
		$str = str_replace("\t", "\\t", $str);
		return $str;
	}

	public function compileArray($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('array', $expression['left']);
	}

	public function compileNewInstance($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('new-instance', $expression);
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
			default:
				throw new Exception("Error Processing Request");
		}


	}

	public function compileIdentical($expression, CompilationContext $compilationContext)
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
					case 'bool':
						if ($right->getCode() == 'true') {
							return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')');
						} else {
							return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')');
						}
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
			default:
				throw new Exception("Error Processing Request");
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
			case 'bool':
				return new CompiledExpression('bool', $expression['value']);
			case 'string':
				return new CompiledExpression('string', $this->_addSlaches($expression['value']));
			case 'variable':
				return new CompiledExpression('variable', $expression['value']);
			case 'empty-array':
				return new CompiledExpression('empty-array', null);
			case 'array':
				return $this->compileArray($expression, $compilationContext);
			case 'new':
				return $this->compileNewInstance($expression, $compilationContext);
			case 'equals':
				return $this->compileEquals($expression, $compilationContext);
			case 'identical':
				return $this->compileIdentical($expression, $compilationContext);
			default:
				throw new Exception("Unknown " . $type . " " . print_r($expression, true));
		}
		//echo $type;
	}

}