<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
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

		/**
		 * Discard first level parentheses
		 */
		if ($exprRaw['type'] == 'list') {
			$expr = new Expression($exprRaw['left']);
		} else {
			$expr = new Expression($exprRaw);
		}
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
				$variableRight = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $exprRaw);
				switch ($variableRight->getType()) {
					case 'int':
						return $variableRight->getName();
					case 'string':
						return $variableRight->getName() . ' && Z_STRLEN_P(' . $variableRight->getName() . ')';
					case 'bool':
						return $variableRight->getName();
					case 'double':
						return $variableRight->getName();
					case 'variable':
						$compilationContext->headersManager->add('kernel/operators');
						if ($variableRight->isLocalOnly()) {
							return 'zephir_is_true(&' . $variableRight->getName() . ')';
						} else {
							return 'zephir_is_true(' . $variableRight->getName() . ')';
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