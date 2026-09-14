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

namespace Zephir\Test;

use FilesystemIterator;
use PHPUnit\Framework\TestCase;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use SplFileInfo;

use function dirname;
use function file_get_contents;
use function preg_match_all;
use function substr;
use function substr_count;

use const PREG_OFFSET_CAPTURE;

/**
 * `kernel/main.h` defines `SL(str)` as `ZEND_STRL` (`str, sizeof(str) - 1`) and
 * `SS(str)` as `ZEND_STRS` (`str, sizeof(str)`). Every kernel helper taking a
 * C string literal wants its byte count, so `SS()` hands over a length that
 * counts the trailing NUL: one byte longer than any key a PHP array can hold,
 * or one byte longer than the name being looked up. Nothing warns. The call
 * simply matches nothing.
 *
 * Three `SS()` emissions survived for years in `Backend::arrayUnset2()`,
 * `Backend::arrayIsset()` and `Backend::arrayIssetFetch2()` because each sat
 * behind a correct `SL()` twin that returned first, so no generated C ever
 * contained one and no test could reach one. This test reads the emitters
 * rather than running them, which is the only way a branch nobody reaches can
 * still be held to the contract.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2708
 */
final class EmittedLengthMacroTest extends TestCase
{
    public function emitterFileProvider(): array
    {
        $cases = [];
        foreach ($this->sourceFiles() as $file) {
            $cases[$this->relativePath($file)] = [$file];
        }

        self::assertNotEmpty($cases);

        return $cases;
    }

    /**
     * @dataProvider emitterFileProvider
     */
    public function testNoEmittedCodeUsesTheNulCountingLengthMacro(string $file): void
    {
        $this->assertSame(
            [],
            $this->nulCountingMacros((string) file_get_contents($file)),
            $this->relativePath($file) . ' emits SS("…"), whose length counts the trailing NUL. '
            . 'Use SL("…") for a C string literal.'
        );
    }

    /**
     * `SS("` is the only spelling that can reach generated C with a literal;
     * the bare token also appears inside longer identifiers such as
     * `ZEPHIR_REGISTER_CLASS(`, which is why the opening quote is part of the
     * pattern.
     *
     * @return list<string>
     */
    private function nulCountingMacros(string $source): array
    {
        preg_match_all('/\bSS\("/', $source, $matches, PREG_OFFSET_CAPTURE);

        $found = [];
        foreach ($matches[0] as [, $offset]) {
            $found[] = 'line ' . (substr_count(substr($source, 0, $offset), "\n") + 1);
        }

        return $found;
    }

    private function relativePath(string $file): string
    {
        return 'src' . substr($file, strlen($this->sourceDir()));
    }

    /**
     * @return list<string>
     */
    private function sourceFiles(): array
    {
        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($this->sourceDir(), FilesystemIterator::SKIP_DOTS)
        );

        $files = [];
        /** @var SplFileInfo $file */
        foreach ($iterator as $file) {
            if ('php' === $file->getExtension()) {
                $files[] = $file->getPathname();
            }
        }

        sort($files);

        return $files;
    }

    private function sourceDir(): string
    {
        return dirname(__DIR__, 2) . '/src';
    }
}
