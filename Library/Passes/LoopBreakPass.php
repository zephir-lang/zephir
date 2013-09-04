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
 * LoopBreakPass
 *
 * This pass checks if the statement block related to a 'loop' loop has at least a 'break'
 * statement disallowing full infinite loops
 */
class LoopBreakPass
{

	protected $_hasBreak = false;

	/**
	 * Do the compilation pass
	 *
	 * @param StatementsBlock $block
	 */
	public function pass(StatementsBlock $block)
	{
		$this->passStatementBlock($block->getStatements());
	}

	public function passStatementBlock(array $statements)
	{
		foreach ($statements as $statement) {
			switch ($statement['type']) {
				case 'if':
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					if (isset($statement['else_statements'])) {
						$this->passStatementBlock($statement['else_statements']);
					}
					break;
				case 'break':
				case 'return':
				case 'throw':
					$this->_hasBreak = true;
					break;
			}
		}
	}
	public function hasBreak()
	{
		return $this->_hasBreak;
	}

}