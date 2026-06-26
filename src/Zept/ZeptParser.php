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
 * Parses the `.zept` (Zephir Test) file format.
 *
 * The format mirrors PHP's `.phpt`: a sequence of `--NAME--` delimited
 * sections. Section bodies are everything between a marker line and the next
 * marker (or end of file).
 *
 * Sections:
 *   --TEST--     (required) one-line human description
 *   --FILE--     (required, repeatable) Zephir source; one class/interface each
 *   --USAGE--    (required) PHP code exercising the compiled extension
 *   --EXPECT--   expected stdout, matched literally
 *   --EXPECTF--  expected stdout, matched with sprintf-style placeholders
 *   --SKIPIF--   (optional) PHP code; printing "skip ..." skips the test
 *   --INI--      (optional) php.ini directives, one `key=value` per line
 *
 * Exactly one of --EXPECT-- / --EXPECTF-- is required.
 */
final class ZeptParser
{
    private const MARKER = '/^--([A-Z][A-Z0-9_]*)--\s*$/';

    /** @var list<string> */
    private const KNOWN = ['TEST', 'FILE', 'USAGE', 'EXPECT', 'EXPECTF', 'SKIPIF', 'INI'];

    /** Sections that may appear more than once. */
    private const REPEATABLE = ['FILE'];

    public function parse(string $raw, string $path): ZeptFile
    {
        $sections = $this->splitSections($raw);

        foreach ($sections as $name => $bodies) {
            if (!in_array($name, self::KNOWN, true)) {
                throw ZeptParseException::in($path, "unknown section --{$name}--");
            }
            if (count($bodies) > 1 && !in_array($name, self::REPEATABLE, true)) {
                throw ZeptParseException::in($path, "duplicate --{$name}-- section (only --FILE-- may repeat)");
            }
        }

        foreach (['TEST', 'FILE', 'USAGE'] as $required) {
            if (!isset($sections[$required])) {
                throw ZeptParseException::in($path, "missing required --{$required}-- section");
            }
        }

        $hasExpect  = isset($sections['EXPECT']);
        $hasExpectF = isset($sections['EXPECTF']);
        if ($hasExpect && $hasExpectF) {
            throw ZeptParseException::in($path, 'both --EXPECT-- and --EXPECTF-- present; use exactly one');
        }
        if (!$hasExpect && !$hasExpectF) {
            throw ZeptParseException::in($path, 'missing required --EXPECT-- or --EXPECTF-- section');
        }

        $files    = $sections['FILE'] ?? [];
        $isFormat = $hasExpectF;
        $expect   = $isFormat ? $sections['EXPECTF'][0] : ($sections['EXPECT'][0] ?? '');

        return new ZeptFile(
            trim($sections['TEST'][0] ?? ''),
            $files,
            $sections['USAGE'][0] ?? '',
            $expect,
            $isFormat,
            isset($sections['SKIPIF']) ? $sections['SKIPIF'][0] : null,
            isset($sections['INI']) ? $sections['INI'][0] : null,
        );
    }

    /**
     * Split the raw file into sections.
     *
     * @return array<string, list<string>> Section name => list of bodies. Only
     *                                      repeatable sections (FILE) ever hold
     *                                      more than one body.
     */
    private function splitSections(string $raw): array
    {
        $lines = explode("\n", str_replace(["\r\n", "\r"], "\n", $raw));

        /** @var array<string, list<string>> $sections */
        $sections = [];
        $current  = null;
        $buffer   = [];

        foreach ($lines as $line) {
            if (preg_match(self::MARKER, $line, $m) === 1) {
                if ($current !== null) {
                    $sections[$current][] = implode("\n", $buffer);
                }
                $current = $m[1];
                $buffer  = [];
                continue;
            }

            if ($current !== null) {
                $buffer[] = $line;
            }
        }

        if ($current !== null) {
            $sections[$current][] = implode("\n", $buffer);
        }

        return $sections;
    }
}
