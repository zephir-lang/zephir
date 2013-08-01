<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

require 'Library/Operators/Arithmetical/BaseOperator.php';
require 'Library/Operators/Arithmetical/AddOperator.php';
require 'Library/Operators/Arithmetical/SubOperator.php';

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
		return new CompiledExpression('array', $expression['left'], $expression);
	}

	public function compileNewInstance($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('new-instance', $expression, $expression);
	}

	public function compileEquals($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($right->getType()) {
					case 'int':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
					case 'null':
						$compilationContext->headersManager->add('kernel/operators');
						return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' == ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' == (int) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
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

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' == ' . $right->getCode() . ')', $expression);
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression);
								}
							case 'null':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') == IS_NULL', $expression);
							default:
								throw new CompilerException("Error Processing Request", $expression);
						}
						break;
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' == ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' == (int) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}

	public function compileLess($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['value']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' < ' . $right->getCode() . ')', $expression);
					case 'double':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' < ' . $right->getCode() . ')', $expression);
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression);
								}
							default:
								throw new CompilerException("Error Processing Request", $expression);
						}
						break;
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' < ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' < (long) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'double':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(double)' . $left->getCode() . ' < ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' < ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}

	public function compileNot($expression, CompilationContext $compilationContext)
	{
		return new CompiledExpression('bool', '', $expression);
	}

	public function compileIsset($expression, CompilationContext $compilationContext)
	{
		$variable = $compilationContext->symbolTable->getVariableForRead($expression['left']['left']['value']);

		switch ($expression['left']['type']) {
			case 'array-access':
				switch ($expression['left']['right']['type'])	{
					case 'string':
						return new CompiledExpression('int', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $expression['left']['right']['value'] . '"))', $expression);
					default:
						echo '[', $expression['left']['right']['type'], ']';
				}
				break;
			default:
				echo '[', $expression['left']['type'], ']';
		}

		return new CompiledExpression('int', '', $expression);
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
			case 'null':
				return new CompiledExpression('null', null, $expression);
			case 'int':
				return new CompiledExpression('int', $expression['value'], $expression);
			case 'double':
				return new CompiledExpression('double', $expression['value'], $expression);
			case 'bool':
				return new CompiledExpression('bool', $expression['value'], $expression);
			case 'string':
				return new CompiledExpression('string', $this->_addSlaches($expression['value']), $expression);
			case 'variable':
				return new CompiledExpression('variable', $expression['value'], $expression);
			case 'empty-array':
				return new CompiledExpression('empty-array', null, $expression);
			case 'array-access':
				return new CompiledExpression('array-access', null, $expression);
			case 'property-access':
				return new CompiledExpression('property-access', null, $expression);
			case 'fcall':
				return new CompiledExpression('fcall', null, $expression);
			case 'mcall':
				return new CompiledExpression('mcall', null, $expression);
			case 'isset':
				return $this->compileIsset($expression, $compilationContext);
			case 'typeof':
				return new CompiledExpression('typeof', null, $expression);
			case 'array':
				return $this->compileArray($expression, $compilationContext);
			case 'new':
				return $this->compileNewInstance($expression, $compilationContext);
			case 'equals':
				return $this->compileEquals($expression, $compilationContext);
			case 'not':
				return $this->compileNot($expression, $compilationContext);
			case 'identical':
				return $this->compileIdentical($expression, $compilationContext);
			case 'not-identical':
				return $this->compileIdentical($expression, $compilationContext);
			case 'less':
				return $this->compileLess($expression, $compilationContext);
			case 'add':
				$expr = new AddOperator();
				return $expr->compile($expression, $compilationContext);
			case 'sub':
				$expr = new SubOperator();
				return $expr->compile($expression, $compilationContext);
			default:
				throw new CompilerException("Unknown expression: " . $type, $expression);
		}
		//echo $type;
	}

}
