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
		if ($expr['type'] == 'not') {
			$conditions = $this->optimize($expr['left'], $compilationContext);
			if ($conditions !== false) {
				return '!(' . $conditions . ')';
			}
		}
		return false;
	}

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

			switch ($expr['right']['type']) {
				case 'array':
					$condition = 'Z_TYPE_P(' . $expr['left']['value'] . ') ' . $operator . ' IS_ARRAY';
					break;
				case 'string':
					$condition = 'Z_TYPE_P(' . $expr['left']['left']['value'] . ') ' . $operator . ' IS_STRING';
					break;
			}

			return $condition;
		}

		return false;
	}

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
		$compiledExpression = $expr->compile($compilationContext);

		/**
		 * Generate the condition according to the value returned by the evaluted expression
		 */
		switch ($compiledExpression->getType()) {
			case 'int':
			case 'double':
				return $compiledExpression->getCode();
			case 'bool':
				return $compiledExpression->getBooleanCode();
			case 'variable':
				$variableRight = $compilationContext->symbolTable->getVariableForRead($exprRaw['value'], $exprRaw);
				switch ($variableRight->getType()) {
					case 'int':
						return $variableRight->getName();
					case 'bool':
						return $variableRight->getName();
					case 'double':
						return $variableRight->getName();
					case 'variable':
						return 'zend_is_true(' . $variableRight->getName() . ')';
					default:
						throw new CompiledException("Variable can't be evaluated " . $variableRight->getType(), $exprRaw);
				}
				break;
			case 'isset':
				$compilationContext->codePrinter->output('//missing');
				return '';
			case 'fcall':
				$compilationContext->codePrinter->output('//missing');
				return '';
			default:
				throw new CompilerException("Expression can't be evaluated", $exprRaw);
		}
	}

}