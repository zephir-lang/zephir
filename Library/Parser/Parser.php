<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Parser;

use Zephir\Exception\IllegalStateException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\ParseException;

use function function_exists;

class Parser
{
    /**
     * Check if Zephir Parser available.
     *
     * @return bool
     */
    public function isAvailable(): bool
    {
        return function_exists('zephir_parse_file');
    }

    /**
     * Gets Zephir Parser version.
     *
     * @return string|null
     */
    public function getVersion(): ?string
    {
        return phpversion('zephir_parser') ?: null;
    }

    /**
     * Parses a file and returning an intermediate representation (IR).
     *
     * @param string $filePath Absolute path to the *.zep file
     *
     * @return array
     *
     * @throws InvalidArgumentException
     * @throws ParseException
     * @throws IllegalStateException
     */
    public function parse(string $filePath): array
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
