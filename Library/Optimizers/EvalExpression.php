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
 * Resolves evaluation of expressions returning a C-int expression that can be used by 'if'/'while'/'do-while' statements
 */
class EvalExpression
{

	protected $_unrecheable = false;

	protected $_unrecheableElse = false;

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
				$this->_unrecheable = true;
				return '0';
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'double':
				$code = $compiledExpression->getCode();
				if (is_numeric($code)) {
					if ($code == '1') {
						$this->_unrecheableElse = true;
					} else {
						$this->_unrecheable = true;
					}
				}

				return $code;
			case 'char':
			case 'uchar':
				return $compiledExpression->getCode();
			case 'bool':
				$code = $compiledExpression->getBooleanCode();
				if ($code == '1') {
					$this->_unrecheableElse = true;
				} else {
					if ($code == '0') {
						$this->_unrecheable = true;
					}
				}
				return $code;
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

	/**
	 * Checks if the evaluation produce unrecheable code
	 *
	 * @return boolean
	 */
	public function isUnrecheable()
	{
		return $this->_unrecheable;
	}

	/**
	 * Checks if the evaluation not produce unrecheable code
	 *
	 * @return boolean
	 */
	public function isUnrecheableElse()
	{
		return $this->_unrecheableElse;
	}

}