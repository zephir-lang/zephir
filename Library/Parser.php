<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | https://zephir-lang.com/license.html                                     |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Parser\ParseException;
use Zephir\Parser\SystemException;

/**
 * Zephir\Parser
 *
 * @package Zephir
 */
class Parser
{
    /**
     * Parses a file and returning an intermediate array representation.
     *
     * @param string $filepath Absolute path to the *.zep file
     * @return array
     * @throws SystemException
     * @throws ParseException
     */
    public function parse($filepath)
    {
        if (!function_exists('zephir_parse_file')) {
            throw new SystemException("Zephir Parser extension couldn't be loaded.");
        }

        if (!is_file($filepath) || !is_readable($filepath)) {
            throw new ParseException('Unable to locate source file to parse.');
        }

        if (!$content = file_get_contents($filepath)) {
            throw new ParseException('Unable to read source file to parse.');
        }

        return zephir_parse_file($content, $filepath);
    }
}
