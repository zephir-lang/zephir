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

use function array_keys;
use function dirname;
use function file_get_contents;
use function in_array;
use function is_array;
use function preg_match_all;
use function sort;
use function strlen;
use function substr;
use function token_get_all;

use const T_CONSTANT_ENCAPSED_STRING;
use const T_ENCAPSED_AND_WHITESPACE;
use const T_INLINE_HTML;

/**
 * A kernel function reaches generated C as text inside a PHP string, so
 * nothing on the PHP side can tell that the name resolves to anything. The
 * only reader is the C compiler, and it only reads the branches a test
 * actually reaches.
 *
 * `zephir_set_symbol_str()` shows what that costs. It was deleted from
 * `kernel/memory.c` in 0cfdd9040 as unused, because the only caller was a
 * string in `src/Statements/Let/ExportSymbolString.php`, which no grep over
 * `kernel/` or `ext/` could see. From 0.18.0 on, `let {"name"} = value`
 * emitted C that does not link, and no test noticed.
 *
 * This test reads the emitters instead of running them, which is the only way
 * to hold a branch nobody reaches to the contract.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2710
 * @see https://github.com/zephir-lang/zephir/issues/2707
 */
final class EmittedKernelSymbolTest extends TestCase
{
    /**
     * Names that are legitimately absent from the static `kernel/` tree.
     *
     * `zephir_concat_function` is generated per project into
     * `ext/kernel/concat.{c,h}` by `Zephir\Backend\StringsManager`, together
     * with the `ZEPHIR_CONCAT_*` family.
     *
     * `zephir_pch_probe` is a function *definition* written into a throwaway
     * translation unit that checks whether GCC accepts the precompiled header
     * (`Compiler::precompiledHeaderIsUsable()`). It is never called.
     */
    private const NOT_IN_KERNEL_TREE = [
        'zephir_concat_function',
        'zephir_pch_probe',
    ];

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
    public function testEveryEmittedKernelSymbolIsDeclared(string $file): void
    {
        $this->assertSame(
            [],
            $this->undeclaredSymbols((string) file_get_contents($file)),
            $this->relativePath($file) . ' emits a kernel function that no header declares. '
            . 'The generated C will not compile once that branch is reached.'
        );
    }

    public function testTheKernelDeclarationHarvestFindsAWellKnownSymbol(): void
    {
        $this->assertContains(
            'zephir_array_isset_string_fetch',
            $this->declaredKernelSymbols(),
            'The kernel header scan returned nothing recognisable, so the test above proves nothing.'
        );
    }

    /**
     * Every `zephir_*(` written inside a PHP string, which is the only way a
     * name can reach generated C. Comments are excluded by construction: the
     * tokenizer hands back `T_COMMENT` separately, and several optimizers do
     * name kernel functions in prose.
     *
     * @return list<string>
     */
    private function undeclaredSymbols(string $source): array
    {
        $declared = $this->declaredKernelSymbols();

        $undeclared = [];
        foreach (token_get_all($source) as $token) {
            if (!is_array($token)) {
                continue;
            }

            [$id, $text, $line] = $token;
            if (
                T_CONSTANT_ENCAPSED_STRING !== $id
                && T_ENCAPSED_AND_WHITESPACE !== $id
                && T_INLINE_HTML !== $id
            ) {
                continue;
            }

            preg_match_all('/\b(zephir_[a-z0-9_]+)\s*\(/', $text, $matches);
            foreach ($matches[1] as $name) {
                if (in_array($name, $declared, true) || in_array($name, self::NOT_IN_KERNEL_TREE, true)) {
                    continue;
                }

                $undeclared[] = 'line ' . $line . ': ' . $name . '()';
            }
        }

        return $undeclared;
    }

    /**
     * Prototypes and macros from `kernel/**.h`. Headers only: a function
     * defined in a `.c` without a prototype is not callable from generated
     * code either.
     *
     * @return list<string>
     */
    private function declaredKernelSymbols(): array
    {
        static $declared = null;

        if (null !== $declared) {
            return $declared;
        }

        $names = [];
        foreach ($this->kernelHeaders() as $header) {
            $source = (string) file_get_contents($header);

            preg_match_all('/\b(zephir_[A-Za-z0-9_]+)\s*\(/', $source, $prototypes);
            preg_match_all('/^\s*#\s*define\s+(zephir_[A-Za-z0-9_]+)/m', $source, $macros);

            foreach ([$prototypes[1], $macros[1]] as $found) {
                foreach ($found as $name) {
                    $names[$name] = true;
                }
            }
        }

        $declared = array_keys($names);
        sort($declared);

        return $declared;
    }

    /**
     * @return list<string>
     */
    private function kernelHeaders(): array
    {
        return $this->filesWithExtension(dirname(__DIR__, 2) . '/kernel', 'h');
    }

    /**
     * @return list<string>
     */
    private function sourceFiles(): array
    {
        return $this->filesWithExtension($this->sourceDir(), 'php');
    }

    /**
     * @return list<string>
     */
    private function filesWithExtension(string $directory, string $extension): array
    {
        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($directory, FilesystemIterator::SKIP_DOTS)
        );

        $files = [];
        /** @var SplFileInfo $file */
        foreach ($iterator as $file) {
            if ($extension === $file->getExtension()) {
                $files[] = $file->getPathname();
            }
        }

        sort($files);

        return $files;
    }

    private function relativePath(string $file): string
    {
        return 'src' . substr($file, strlen($this->sourceDir()));
    }

    private function sourceDir(): string
    {
        return dirname(__DIR__, 2) . '/src';
    }
}
