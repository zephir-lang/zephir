<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Parser;

use Zephir\Exception\IllegalStateException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\ParseException;

class Parser
{
    /**
     * Check if Zephir Parser available.
     *
     * @return bool
     */
    public function isAvailable()
    {
        return \function_exists('zephir_parse_file');
    }

    /**
     * Gets Zephir Parser version.
     *
     * @return string|null
     */
    public function getVersion()
    {
        return phpversion('Zephir Parser') ?: null;
    }

    /**
     * Parses a file and returning an intermediate representation (IR).
     *
     * @param string $filePath Absolute path to the *.zep file
     *
     * @throws IllegalStateException
     * @throws InvalidArgumentException
     * @throws ParseException
     *
     * @return array
     */
    public function parse($filePath)
    {
        if (!$this->isAvailable()) {
            throw new IllegalStateException("Zephir Parser extension couldn't be loaded.");
        }

        if (!is_file($filePath) || !is_readable($filePath)) {
            throw new InvalidArgumentException('Unable to locate source file to parse.');
        }

        $content = file_get_contents($filePath);

        if (false === $content) {
            throw new ParseException('Unable to read source file to parse.');
        }

        return zephir_parse_file($content, $filePath);
    }
}
