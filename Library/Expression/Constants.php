<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
	 * Reserved ENV Constants
	 * @link http://www.php.net/manual/ru/reserved.constants.php
	 * @var array
	 */
	protected $envConstans = array(
		'PHP_VERSION',
		'PHP_MAJOR_VERSION',
		'PHP_MINOR_VERSION',
		'PHP_RELEASE_VERSION',
		'PHP_VERSION_ID',
		'PHP_EXTRA_VERSION',
		'PHP_ZTS',
		'PHP_DEBUG',
		'PHP_MAXPATHLEN',
		'PHP_OS',
		'PHP_SAPI',
		'PHP_EOL',
		'PHP_INT_MAX',
		'PHP_INT_SIZE',
		'DEFAULT_INCLUDE_PATH',
		'PHP_BINDIR',
		'PHP_LIBDIR',
		'PHP_DATADIR',
	);

	/**
	 * Magick constants
	 * @link http://php.net/manual/en/language.constants.predefined.php
	 * @var array
	 */
	protected $magickConstants = array(
		'__LINE__',
		'__FILE__',
		'__DIR__',
		'__FUNCTION__',
		'__CLASS__',
		'__TRAIT__',
		'__METHOD__',
		'__NAMESPACE__'
	);

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

		$value = &$expression['value'];
		$mergedConstants = array_merge($this->envConstans, $this->magickConstants);

		if (!defined($expression['value']) && !in_array($value, $mergedConstants)) {
			if (!$compilationContext->compiler->isConstant($value)) {
				$compilationContext->logger->warning("Constant \"" . $value . "\" does not exist at compile time", "nonexistant-constant", $expression);
			} else {
				$isZephirConstant = true;
			}
		} else {
			$isPhpConstant = true;
		}

		if ($isPhpConstant && !in_array($value, $mergedConstants)) {
			$value = constant($value);
			$type = strtolower(gettype($value));

			switch ($type) {
				case 'integer':
					return new CompiledExpression('int', $value, $expression);
				case 'string':
					return new CompiledExpression('string', Utils::addSlashes($value), $expression);
				default:
					return new CompiledExpression($type, $value, $expression);
					break;
			}
		}

		if ($isZephirConstant) {
			$constant = $compilationContext->compiler->getConstant($value);
			return new CompiledExpression($constant[0], $constant[1], $expression);
		}

		if (in_array($value, $this->magickConstants)) {
			$compilationContext->logger->warning("Constant \"" . $value . "\" is not supported magic constant", 'notsupported-magic-constant', $expression);
		}

		$symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
		$compilationContext->codePrinter->output('ZEPHIR_GET_CONSTANT('.$symbolVariable->getName().', "'.$expression['value'].'");');
		return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
	}

}