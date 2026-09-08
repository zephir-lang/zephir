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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * Zephir's `long`/`ulong` describe a PHP `int`, which the engine keeps in a
 * `zend_long` (`int64_t`). Emitting a C `long` instead makes the width follow
 * the data model, so the local is 64-bit on LP64 (Linux, macOS) and 32-bit on
 * LLP64 (Windows x64).
 *
 * For a parameter that is worse than a narrowing conversion: `Z_PARAM_LONG(n)`
 * expands to `zend_parse_arg_long(arg, &n, ...)`, whose `dest` is a
 * `zend_long *`. A `long *` to a 4-byte object receives an 8-byte store.
 *
 * The assertions here run on every platform because they inspect the generated
 * C rather than its behaviour, which is what makes a Windows-only bug testable
 * from a Linux container.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2666
 */
final class Issue2666Test extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testLongAndULongEmitFixedWidthCTypes(): void
    {
        $generated = $this->generate(
            'longwidth',
            'widths.zep',
            <<<'ZEP'
                namespace Longwidth;

                class Widths
                {
                    public function fromParam(long n) -> string
                    {
                        string retval = "n=";
                        let retval .= n;
                        return retval;
                    }

                    public function fromUnsignedParam(ulong u) -> string
                    {
                        string retval = "u=";
                        let retval .= u;
                        return retval;
                    }

                    public function locals(var value) -> long
                    {
                        long total = 0;
                        ulong step = 1;

                        let total = (int) value;
                        let total = total + step;

                        return total;
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('zend_long n;', $generated);
        $this->assertStringContainsString('zend_ulong u;', $generated);
        $this->assertStringContainsString('Z_PARAM_LONG(n)', $generated);

        $this->assertSame(
            [],
            $this->platformVariantDeclarations($generated),
            'Generated C declares a local whose width follows the data model.',
        );
    }

    /**
     * `zephir_array_update_multi()` and its two siblings read every `l` offset
     * with `va_arg(ap, zend_long)`. A variadic argument passed as a plain C
     * `int` therefore has its upper half read as whatever the ABI left in the
     * slot, and a `uint`/`ulong` local reached the slot as `&name`. Neither is
     * observable on the System V ABI, where the compiler happens to clear the
     * upper half, so the contract is asserted on the generated C instead.
     */
    public function testMultiDimensionalIndexesArePassedAsZendLong(): void
    {
        $generated = $this->generate(
            'multiwidth',
            'keys.zep',
            <<<'ZEP'
                namespace Multiwidth;

                class Keys
                {
                    public function literals() -> array
                    {
                        array result = [];
                        let result[0][1] = "v";
                        return result;
                    }

                    public function unsignedLocal(uint key) -> array
                    {
                        array result = [];
                        let result[key][2] = "v";
                        return result;
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString(
            'zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) 0, (zend_long) 1);',
            $generated,
        );
        $this->assertStringContainsString(
            'zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) key, (zend_long) 2);',
            $generated,
        );
    }

    /**
     * Declarations whose C type is a bare `long`/`unsigned long`, i.e. 32-bit
     * under LLP64 and 64-bit under LP64. `zend_long`/`zend_ulong` are excluded
     * by the leading word boundary.
     *
     * @return list<string>
     */
    private function platformVariantDeclarations(string $source): array
    {
        preg_match_all(
            '/^\h*(?:unsigned\h+)?long\h+[A-Za-z_]\w*\s*[;,=]/m',
            $source,
            $matches,
        );

        return array_map('trim', $matches[0]);
    }

    /**
     * Scaffolds a throwaway project, drops a single `.zep` in it, runs
     * `zephir generate` and returns the generated C source.
     */
    private function generate(string $project, string $fileName, string $source): string
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/' . $fileName, $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf(
            '%s/ext/%s/%s.c',
            $projectDir,
            $project,
            basename($fileName, '.zep') . '.zep',
        );
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }
}
