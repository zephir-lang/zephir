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

use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\ParseException;
use Zephir\Parser\Php\PhpParser;

use function file_get_contents;
use function function_exists;
use function getenv;
use function is_file;
use function is_readable;
use function phpversion;

class Parser
{
    /**
     * Environment switch that forces the pure-PHP parser even when the
     * `ext-zephir_parser` C extension is loaded. Used for differential testing
     * and for users who want a deterministic, dependency-free backend.
     */
    public const FORCE_PHP_ENV = 'ZEPHIR_FORCE_PHP_PARSER';

    /**
     * Gets Zephir Parser (C extension) version, or null when not loaded.
     *
     * @return string|null
     */
    public function getVersion(): ?string
    {
        return phpversion('zephir_parser') ?: null;
    }

    /**
     * Whether a parser backend is available.
     *
     * Always true now: the pure-PHP parser ships with Zephir, so parsing no
     * longer depends on the optional C extension.
     */
    public function isAvailable(): bool
    {
        return true;
    }

    /**
     * Whether the C extension fast-path is loaded.
     */
    public function isExtensionLoaded(): bool
    {
        return function_exists('zephir_parse_file');
    }

    /**
     * Parses a file and returning an intermediate representation (IR).
     *
     * Backend selection: the pure-PHP parser is used when forced via
     * {@see FORCE_PHP_ENV}; otherwise the C extension is used as a fast path
     * when loaded; otherwise the pure-PHP parser is the always-available
     * fallback. All backends produce the identical IR.
     *
     * @param string $filePath Absolute path to the *.zep file
     *
     * @return array
     *
     * @throws InvalidArgumentException
     * @throws ParseException
     */
    public function parse(string $filePath): array
    {
        if (!is_file($filePath) || !is_readable($filePath)) {
            throw new InvalidArgumentException('Unable to locate source file to parse.');
        }

        $content = file_get_contents($filePath);

        if (false === $content) {
            throw new ParseException('Unable to read source file to parse.');
        }

        if (!getenv(self::FORCE_PHP_ENV) && function_exists('zephir_parse_file')) {
            return zephir_parse_file($content, $filePath);
        }

        return (new PhpParser())->parse($content, $filePath);
    }
}
