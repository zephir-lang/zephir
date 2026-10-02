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
 * A compound assignment on an array element compiles to one read-modify-write
 * kernel call, for every operator PHP accepts there, including the bitwise
 * ones. On a declared `string` the operation could never succeed, so PHP's
 * runtime Error is raised at build time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747Test extends TestCase
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

    public static function stringTargetProvider(): array
    {
        return [
            'single offset'  => ['let s[0] .= "x";', 'Cannot use assign-op operators with string offsets'],
            'append'         => ['let s[] .= "x";', '[] operator not supported for strings'],
            'chained offset' => ['let s[0][1] .= "x";', 'Cannot use string offset as an array'],
        ];
    }

    /**
     * @dataProvider stringTargetProvider
     */
    public function testCompoundAssignmentOnAStringIsRejected(string $statement, string $message): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'compoundstring',
            "    public function run(string s)\n    {\n        " . $statement . "\n    }",
        );

        $this->assertSame(1, $exitCode, $stderr);
        $this->assertStringContainsString($message, $stderr);
    }

    public function testEveryFormCompilesToTheReadModifyWriteCall(): void
    {
        [$exitCode, $stderr, $generated] = $this->generateProject(
            'compounddim',
            <<<'ZEP'
                    public static sp = [];
                    public p = [];

                    public function run(var a, var k, var v)
                    {
                        let a[k] &= v;
                        let a["k"][1] <<= v;
                        let a[] -= v;
                        let a[k][] .= v;
                        let this->p[k] %= v;
                        let this->p[] *= v;
                        let self::sp[k][] /= v;

                        return a;
                    }
                ZEP,
        );

        $this->assertSame(0, $exitCode, $stderr);
        $this->assertStringContainsString('zephir_array_assign_op(a, v, bitwise_and_function, SL("z"), 1, k);', $generated);
        $this->assertStringContainsString('zephir_array_assign_op(a, v, shift_left_function, SL("sl"), 3, SL("k"), (zend_long) 1);', $generated);
        $this->assertStringContainsString('zephir_array_assign_op(a, v, sub_function, SL("a"), 1);', $generated);
        $this->assertStringContainsString('zephir_array_assign_op(a, v, concat_function, SL("za"), 2, k);', $generated);
        $this->assertStringContainsString('mod_function, SL("z"), 1, k);', $generated);
        $this->assertStringContainsString('mul_function, SL("a"), 1);', $generated);
        $this->assertStringContainsString('div_function, SL("za"), 2, k);', $generated);
        $this->assertStringContainsString('zephir_fetch_property_rw(', $generated);
        $this->assertStringContainsString('zephir_fetch_static_property_rw_ce(', $generated);
        $this->assertStringNotContainsString('zephir_array_update', $generated);
    }

    /**
     * @return array{0: int, 1: string, 2: string} exit code, stderr and the generated C
     */
    private function generateProject(string $name, string $body): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result    = $this->runZephir('generate --no-ansi', $projectDir);
        $generated = $projectDir . '/ext/' . $name . '/sample.zep.c';

        return [
            $result['exitCode'],
            $result['stderr'],
            is_file($generated) ? (string) file_get_contents($generated) : '',
        ];
    }
}
