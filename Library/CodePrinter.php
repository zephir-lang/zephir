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
 * CodePrinter
 *
 * Buffers code, making it look pretty
 */
class CodePrinter
{
	protected $_code;

	protected $_lastLine;

	protected $_level = 0;

	protected $_currentPrints = 0;

	/**
	 * Adds a line to the output without the automatic line feed
	 *
	 * @param string $code
	 */
	public function outputNoLineFeed($code)
	{
		$this->_lastLine = $code;
		$this->_code .= str_repeat("\t", $this->_level) . $code;
	}

	/**
	 * Add code to the output at the beggining
	 *
	 * @param string $code
	 */
	public function preOutput($code)
	{
		$this->_lastLine = $code;
		$this->_code = str_repeat("\t", $this->_level) . $code . PHP_EOL . $this->_code;
		$this->_currentPrints++;
	}

	/**
	 * Adds a line to the output without the automatic line feed
	 *
	 * @param string $code
	 */
	public function preOutputNoLineFeed($code)
	{
		$this->_lastLine = $code;
		$this->_code = str_repeat("\t", $this->_level) . $code . $this->_code;
	}

	/**
	 * Adds code to the output without the indentation level
	 *
	 * @param string $code
	 */
	public function preOutputNoLevel($code)
	{
		$this->_lastLine = $code;
		$this->_code = $code . PHP_EOL . $this->_code;
		$this->_currentPrints++;
	}

	/**
	 * Add code to the output without indentation
	 *
	 * @param string $code
	 */
	public function outputNoIndent($code)
	{
		$this->_lastLine = $code;
		$this->_code .= $code . PHP_EOL;
		$this->_currentPrints++;
	}

	/**
	 * Add code to the output
	 *
	 * @param string $code
	 */
	public function output($code)
	{
		$this->_lastLine = $code;
		$this->_code .= str_repeat("\t", $this->_level) . $code . PHP_EOL;
		$this->_currentPrints++;
	}

	/**
	 * Adds a comment to the output with indentation level
	 *
	 * @param string $docBlock
	 */
	public function outputDocBlock($docblock)
	{
		$code = '';
		$docblock = '/' . $docblock . '/';
		foreach (explode("\n", $docblock) as $line) {
			$code .= preg_replace('/^[ \t]+/', ' ', $line) . PHP_EOL;
		}
		$this->_lastLine = $code;
		$this->_code .= $code;
		$this->_currentPrints++;
	}

	/**
	 * Adds code to the output without the indentation level
	 *
	 * @param string $code
	 */
	public function outputNoLevel($code)
	{
		$this->_lastLine = $code;
		$this->_code .= $code . PHP_EOL;
		$this->_currentPrints++;
	}

	/**
	 * Adds a blank line to the output
	 * Optionally controlling if the blank link must be added if the
	 * previous line added isn't one blank line too
	 *
	 * @param boolean $ifPrevNotBlank
	 */
	public function preOutputBlankLine($ifPrevNotBlank=false)
	{
		if (!$ifPrevNotBlank) {
			$this->_code = PHP_EOL . $this->_code;
			$this->_lastLine = PHP_EOL;
			$this->_currentPrints++;
		} else {
			if (trim($this->_lastLine)) {
				$this->_code = PHP_EOL . $this->_code;
				$this->_lastLine = PHP_EOL;
				$this->_currentPrints++;
			}
		}
	}

	/**
	 * Adds a blank line to the output
	 * Optionally controlling if the blank link must be added if the
	 * previous line added isn't one blank line too
	 *
	 * @param boolean $ifPrevNotBlank
	 */
	public function outputBlankLine($ifPrevNotBlank=false)
	{
		if (!$ifPrevNotBlank) {
			$this->_code .= PHP_EOL;
			$this->_lastLine = PHP_EOL;
			$this->_currentPrints++;
		} else {
			if (trim($this->_lastLine)) {
				$this->_code .= PHP_EOL;
				$this->_lastLine = PHP_EOL;
				$this->_currentPrints++;
			}
		}
	}

	/**
	 * Increase the indentation level
	 */
	public function increaseLevel()
	{
		$this->_level++;
	}

	/**
	 * Decrease the indentation level
	 */
	public function decreaseLevel()
	{
		$this->_level--;
	}

	/**
	 * Returns the output in the buffer
	 *
	 * @return string
	 */
	public function getOutput()
	{
		return $this->_code;
	}

	/**
	 * Returns an approximate number of lines printed by the CodePrinter
	 *
	 * @return int
	 */
	public function getNumberPrints()
	{
		return $this->_currentPrints;
	}

}
