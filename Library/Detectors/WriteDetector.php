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
 * WriteDetector
 *
 * Detects if a variable is written in a given context
 * If a variable is not modified in a local context we could avoid allocate memory for its body
 * Separate parameters to avoid them to be touched modifying its reference count
 *
 * @todo, variable is assigned in the fetch operator and in functions that pass parameters by reference
 */
class WriteDetector
{

	public function passLetStatement($variable, array $statement)
	{
		foreach ($statement['assignments'] as $assigment) {
			if ($assigment['variable'] == $variable) {
				return true;
			}
		}
		return false;
	}

	public function detect($variable, $statements)
	{
		foreach ($statements as $statement) {
			switch ($statement['type']) {
				case 'let':
					if ($this->passLetStatement($variable, $statement) === true) {
						return true;
					}
					break;
				case 'do-while':
				case 'while':
				case 'loop':
					if (isset($statement['statements'])) {
						if ($this->detect($variable, $statement['statements']) === true) {
							return true;
						}
					}
					break;
				case 'if':
					if (isset($statement['statements'])) {
						if ($this->detect($variable, $statement['statements']) === true) {
							return true;
						}
					}
					if (isset($statement['else_statements'])) {
						if ($this->detect($variable, $statement['else_statements']) === true) {
							return true;
						}
					}
					break;
				case 'switch':
					foreach ($statement['clauses'] as $clause) {
						if (isset($clause['statements'])) {
							if ($this->detect($variable, $clause['statements']) === true) {
								return true;
							}
						}
					}
					break;
				case 'for':
					if (isset($statement['key'])) {
						if ($statement['key'] == $variable) {
							return true;
						}
					}
					if (isset($statement['value'])) {
						if ($statement['value'] == $variable) {
							return true;
						}
					}
					if (isset($statement['statements'])) {
						if ($this->detect($variable, $statement['statements']) === true) {
							return true;
						}
					}
					break;
			}
		}
		return false;
	}
}
