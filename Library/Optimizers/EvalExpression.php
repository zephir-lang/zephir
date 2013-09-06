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

/**
 * EvalExpression
 *
 * Resolves evaluation expressions returning a C-int expression that can be used by 'if'/'while' statements
 */
class EvalExpression
{
	public function optimizeNot($expr, $compilationContext)
	{
		/**
		 * Compile the expression negating the evaluted expression
		 */
		if ($expr['type'] == 'not') {
			$conditions = $this->optimize($expr['left'], $compilationContext);
			if ($conditions !== false) {
				return '!(' . $conditions . ')';
			}
		}
		return false;
	}

	/**
	 *
	 * @param array $expr
	 * @param array $compilationContext
	 */
	public function optimizeTypeOf($expr, $compilationContext)
	{

		if (!isset($expr['left'])) {
			return false;
		}

		if ($expr['left']['type'] == 'typeof' && $expr['right']['type'] == 'string') {

			if (isset($expr['type'])) {
				switch ($expr['type']) {
					case 'identical':
					case 'equals':
						$operator = '==';
						break;
					case 'not-identical':
					case 'not-equals':
						$operator = '!=';
						break;
					default:
						return false;
				}
			}

			/** @todo, read left variable from the symbol table */
			switch ($expr['right']['value']) {
				case 'array':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_ARRAY';
					break;
				case 'object':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_OBJECT';
					break;
				case 'null':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_NULL';
					break;
				case 'string':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_STRING';
					break;
				case 'int':
				case 'integer':
				case 'long':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_LONG';
					break;
				default:
					throw new CompilerException($expr['right']['value'], $expr['right']);
			}

			return $condition;
		}

		return false;
	}

	/**
	 * Optimizes expressions
	 *
	 * @param array $exprRaw
	 * @param \CompilationContext $compilationContext
	 */
	public function optimize($exprRaw, CompilationContext $compilationContext)
	{

		$conditions = $this->optimizeNot($exprRaw, $compilationContext);
		if ($conditions !== false) {
			return $conditions;
		}

		$conditions = $this->optimizeTypeOf($exprRaw, $compilationContext);
		if ($conditions !== false) {
			return $conditions;
		}

		$expr = new Expression($exprRaw);
		$expr->setReadOnly(true);
		$compiledExpression = $expr->compile($compilationContext);

		/**
		 * Possible corrupted expression?
		 */
		if (!is_object($compiledExpression)) {
			throw new CompilerException('Corrupted expression: ' . $exprRaw['type'], $exprRaw);
		}

		/**
		 * Generate the condition according to the value returned by the evaluted expression
		 */
		switch ($compiledExpression->getType()) {
			case 'null':
				/**
				 * @TODO This potentially would create unrecheable code
				 */
				return '0';
			case 'int':
			case 'double':
				return $compiledExpression->getCode();
			case 'bool':
				/**
				 * @TODO This potentially would create unrecheable code if is evaluated to false
				 */
				return $compiledExpression->getBooleanCode();
			case 'variable':
				$variableRight = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $exprRaw);
				switch ($variableRight->getType()) {
					case 'int':
						return $variableRight->getName();
					case 'string':
						return $variableRight->getName() . ' && ' . $variableRight->getName() . '->len';
					case 'bool':
						return $variableRight->getName();
					case 'double':
						return $variableRight->getName();
					case 'variable':
						if ($variableRight->isLocalOnly()) {
							return 'zend_is_true(&' . $variableRight->getName() . ')';
						} else {
							return 'zend_is_true(' . $variableRight->getName() . ')';
						}
					default:
						throw new CompilerException("Variable can't be evaluated " . $variableRight->getType(), $exprRaw);
				}
				break;
			default:
				throw new CompilerException("Expression " . $compiledExpression->getType() . " can't be evaluated", $exprRaw);
		}
	}

}