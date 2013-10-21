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
 * Constants
 *
 * Resolves PHP or Zephir constants into C-Code
 */
class Constants
{
	protected $_expecting = true;

	protected $_readOnly = false;

	protected $_expectingVariable;

	/**
	 * Sets if the variable must be resolved into a direct variable symbol
	 * create a temporary value or ignore the return value
	 *
	 * @param boolean $expecting
	 * @param Variable $expectingVariable
	 */
	public function setExpectReturn($expecting, Variable $expectingVariable=null)
	{
		$this->_expecting = $expecting;
		$this->_expectingVariable = $expectingVariable;
	}

	/**
	 * Sets if the result of the evaluated expression is read only
	 *
	 * @param boolean $readOnly
	 */
	public function setReadOnly($readOnly)
	{
		$this->_readOnly = $readOnly;
	}

	/**
	 * Resolves a PHP constant value into C-code
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{
		$isPhpConstant = false;
		$isZephirConstant = false;

		if (!defined($expression['value'])) {
			if (!$compilationContext->compiler->isConstant($expression['value'])) {
				$compilationContext->logger->warning("Constant \"" . $expression['value'] . "\" does not exist at compile time", "nonexistant-constant", $expression);
			} else {
				$isZephirConstant = true;
			}
		} else {
			$isPhpConstant = true;
		}

		if ($isPhpConstant) {
			$value = constant($expression['value']);
			switch (gettype($value)) {
				case 'integer':
					return new CompiledExpression('int', $value, $expression);
				case 'string':
					return new CompiledExpression('string', Utils::addSlashes($value), $expression);
			}
			return new CompiledExpression(strtolower(gettype($value)), $value, $expression);
		}

		if ($isZephirConstant) {
			$constant = $compilationContext->compiler->getConstant($expression['value']);
			return new CompiledExpression($constant[0], $constant[1], $expression);
		}

		/* @todo read the constant every time */
		return new CompiledExpression('null', null, $expression);
	}

}