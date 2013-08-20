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
 * ReadDetector
 *
 * Detects if a variable is used in a given expression context
 * Since zvals are collected between executions to the same section of code
 * We need to ensure that a variable is not contained in the right expression
 * used to assign the variable, avoiding premature initializations
 */
class ReadDetector
{
	public function detect($variable, $expression)
	{
		if ($expression['type'] == 'variable') {
			if ($variable == $expression['value']) {
				return true;
			}
		}

		if ($expression['type'] == 'fcall' || $expression['type'] == 'mcall') {
			if (isset($expression['parameters'])) {
				foreach ($expression['parameters'] as $parameter) {
					if ($parameter['type'] == 'variable') {
						if ($variable == $parameter['value']) {
							return true;
						}
					}
				}
			}
		}

		if (isset($expression['left'])) {
			if ($this->detect($variable, $expression['left']) === true) {
				return true;
			}
		}

		if (isset($expression['right'])) {
			if ($this->detect($variable, $expression['right']) === true) {
				return true;
			}
		}

		return false;
	}
}
