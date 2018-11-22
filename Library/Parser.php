<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\IllegalStateException;
use Zephir\Parser\ParseException;

/**
 * Zephir\Parser
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
     * @return string|null
     */
    public function getVersion()
    {
        return extension_loaded('Zephir Parser') ? phpversion('Zephir Parser') : null;
    }

    /**
     * Parses a file and returning an intermediate representation (IR).
     *
     * @param string $filepath Absolute path to the *.zep file
     * @throws IllegalStateException
     * @throws ParseException
     * @return array
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
