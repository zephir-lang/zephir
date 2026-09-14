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
use function explode;
use function file_get_contents;
use function function_exists;
use function in_array;
use function ltrim;
use function preg_match_all;
use function preg_split;
use function sort;
use function str_contains;
use function strlen;
use function strpos;
use function substr;
use function substr_count;
use function trim;

use const PREG_SET_ORDER;

/**
 * A `use function` import is never checked by PHP until the call runs, and a
 * call that only lives on an unreachable branch never runs. So an import of a
 * function that does not exist can sit in the tree indefinitely, and the file
 * still parses, still passes every test, and still ships.
 *
 * `src/Backend/VariablesManager.php` carried
 * `use function Zephir\Backend\ZendEngine3\add_slashes;` from 2023 to 2026.
 * That namespace has never existed. The two calls behind it were written
 * unqualified in 2020, and an unqualified call falls back to the *global*
 * namespace, never to a parent one, so they could not have reached
 * `Zephir\add_slashes()` either. When `add_slashes()` moved into
 * `Zephir\Name`, a search for the old name did not match this file.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2707
 */
final class ImportedFunctionExistsTest extends TestCase
{
    /**
     * Provided by an optional extension and called only behind a
     * `function_exists()` guard, so the import resolves on a machine that has
     * the extension and is unreachable on one that does not.
     *
     * @see src/Expression/NativeArray.php:100
     */
    private const OPTIONAL_EXTENSION_FUNCTIONS = [
        'gmp_nextprime',
        'gmp_strval',
    ];

    public function sourceFileProvider(): array
    {
        $cases = [];
        foreach ($this->sourceFiles() as $file) {
            $cases[$this->relativePath($file)] = [$file];
        }

        self::assertNotEmpty($cases);

        return $cases;
    }

    /**
     * @dataProvider sourceFileProvider
     */
    public function testEveryImportedFunctionExists(string $file): void
    {
        $this->assertSame(
            [],
            $this->unresolvableImports((string) file_get_contents($file)),
            $this->relativePath($file) . ' imports a function that does not exist. '
            . 'A call to it is a fatal Error the moment its branch is reached.'
        );
    }

    /**
     * One statement can import several names (`use function a, b;`) and each
     * may be aliased (`use function a as b;`), so the statement is taken whole
     * and split rather than matched name by name. A grouped import
     * (`use function A\\{b, c};`) is reported rather than skipped: a form this
     * test cannot read is a hole in it, not a pass.
     *
     * @return list<string>
     */
    private function unresolvableImports(string $source): array
    {
        preg_match_all('/^\s*use\s+function\s+([^;]+);/m', $source, $matches, PREG_SET_ORDER);

        $unresolvable = [];
        foreach ($matches as $match) {
            $line = $this->lineOf($source, $match[0]);

            if (str_contains($match[1], '{')) {
                $unresolvable[] = 'line ' . $line . ': grouped import, teach this test to read it';
                continue;
            }

            foreach (explode(',', $match[1]) as $import) {
                $name = ltrim(trim(preg_split('/\s+as\s+/i', trim($import))[0]), '\\');

                if ('' === $name || function_exists($name)) {
                    continue;
                }

                if (in_array($name, self::OPTIONAL_EXTENSION_FUNCTIONS, true)) {
                    continue;
                }

                $unresolvable[] = 'line ' . $line . ': ' . $name;
            }
        }

        return $unresolvable;
    }

    private function lineOf(string $source, string $needle): int
    {
        $offset = strpos($source, $needle);

        return false === $offset ? 0 : substr_count(substr($source, 0, $offset), "\n") + 1;
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
