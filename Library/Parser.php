<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */


namespace Zephir;

use Zephir\Parser\ParseException;
use Zephir\Exception\IllegalStateException;

/**
 * Zephir\Parser
 *
 * @package Zephir
 */
class Parser
{
    /**
     * Check if Zephir Parser available.
     *
     * @return bool
     */
    public function isAvailable()
    {
        return function_exists('zephir_parse_file');
    }

    /**
     * Gets Zephir Parser version.
     *
     * @return null|string
     */
    public function getVersion()
    {
        return extension_loaded('Zephir Parser') ? phpversion('Zephir Parser') : null;
    }

    /**
     * Parses a file and returning an intermediate representation (IR).
     *
     * @param string $filepath Absolute path to the *.zep file
     * @return array
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function parse($filepath)
    {
        if (!$this->isAvailable()) {
            throw new IllegalStateException("Zephir Parser extension couldn't be loaded.");
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
