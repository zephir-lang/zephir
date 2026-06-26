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

namespace Zephir\Zept;

/**
 * Compares the actual program output of a `.zept` `--USAGE--` run against the
 * expected output, in either literal (`--EXPECT--`) or format (`--EXPECTF--`)
 * mode.
 *
 * Newlines are normalised and trailing whitespace ignored before comparison,
 * matching the leniency of PHP's own `.phpt` runner.
 */
final class OutputMatcher
{
    public function matches(string $expected, string $actual, bool $isFormat): bool
    {
        $expected = rtrim($this->normalizeNewlines($expected));
        $actual   = rtrim($this->normalizeNewlines($actual));

        if (!$isFormat) {
            return $expected === $actual;
        }

        return preg_match($this->buildPattern($expected), $actual) === 1;
    }

    private function normalizeNewlines(string $value): string
    {
        return str_replace(["\r\n", "\r"], "\n", $value);
    }

    /**
     * Translate a `--EXPECTF--` template into an anchored regex.
     *
     * Text between `%r ... %r` markers is treated as a raw (sub)regex; all
     * other text is escaped literally, then the sprintf-style placeholders are
     * expanded. The `s` modifier lets `%a`/`%c` span newlines, matching PHP's
     * own `.phpt` runner.
     */
    private function buildPattern(string $expected): string
    {
        $pattern  = '';
        $segments = explode('%r', $expected);
        foreach ($segments as $index => $segment) {
            $pattern .= $index % 2 === 1
                ? $segment
                : $this->expandSpecifiers(preg_quote($segment, '/'));
        }

        return '/^' . $pattern . '$/s';
    }

    private function expandSpecifiers(string $quoted): string
    {
        return str_replace(
            ['%e', '%s', '%S', '%a', '%A', '%w', '%i', '%d', '%x', '%f', '%c'],
            [
                preg_quote(DIRECTORY_SEPARATOR, '/'),
                '[^\r\n]+',
                '[^\r\n]*',
                '.+',
                '.*',
                '\s*',
                '[+-]?\d+',
                '\d+',
                '[0-9a-fA-F]+',
                '[+-]?\.?\d+\.?\d*(?:[Ee][+-]?\d+)?',
                '.',
            ],
            $quoted
        );
    }
}
