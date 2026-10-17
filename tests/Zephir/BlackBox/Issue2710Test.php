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
 * `let {x} op= v` and `let {"x"} op= v` write to the active PHP symbol table,
 * as `${$x} op= $v` does.
 *
 * The runtime behaviour is covered by `tests/zept/dynamic_symbol_export_ops.zept`.
 * These cases pin what only the compiler decides: a literal `{"this"}` is a
 * compile error, as `${'this'} = 1` is in PHP, and a failed export leaves the
 * method through the exception check instead of a bare `return` that skips
 * the memory frame.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2710
 */
final class Issue2710Test extends TestCase
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

    public function testLiteralThisNameIsACompileError(): void
    {
        [$exitCode, $stderr] = $this->generate(
            'dynthis',
            <<<'ZEP'
                namespace Dynthis;

                class Export
                {
                    public function run(var value) -> void
                    {
                        let {"this"} = value;
                    }
                }
                ZEP,
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Cannot re-assign $this', $stderr);
    }

    public function testCompoundOperatorsReachTheKernel(): void
    {
        [$exitCode, $stderr, $generated] = $this->generate(
            'dynops',
            <<<'ZEP'
                namespace Dynops;

                class Export
                {
                    public function run(var name, var value) -> void
                    {
                        let {name} += value;
                        let {"literal"} .= value;
                        let {name} <<= 2;
                    }
                }
                ZEP,
        );

        $this->assertSame(0, $exitCode, $stderr);
        $this->assertMatchesRegularExpression('/zephir_set_symbol_op\(name, &_\d+, add_function\)/', $generated);
        $this->assertMatchesRegularExpression(
            '/zephir_set_symbol_str_op\(SL\("literal"\), &_\d+, concat_function\)/',
            $generated,
        );
        $this->assertMatchesRegularExpression('/zephir_set_symbol_op\(name, &_\d+, shift_left_function\)/', $generated);
    }

    public function testFailedExportLeavesThroughTheExceptionCheck(): void
    {
        [$exitCode, $stderr, $generated] = $this->generate(
            'dynexc',
            <<<'ZEP'
                namespace Dynexc;

                class Export
                {
                    public function run(var name, var value) -> void
                    {
                        let {name} = value;
                        let {"literal"} = value;
                    }
                }
                ZEP,
        );

        $this->assertSame(0, $exitCode, $stderr);
        $this->assertStringNotContainsString('== FAILURE', $generated);
        $this->assertMatchesRegularExpression(
            '/zephir_set_symbol\(name, &_\d+\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $generated,
        );
        $this->assertMatchesRegularExpression(
            '/zephir_set_symbol_str\(SL\("literal"\), &_\d+\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $generated,
        );
    }

    /**
     * Scaffolds a throwaway project with one `.zep`, runs `zephir generate`
     * and returns its exit code, stderr and the generated C source.
     *
     * @return array{0: int, 1: string, 2: string}
     */
    private function generate(string $project, string $source): array
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/export.zep', $source . "\n");

        $result        = $this->runZephir('generate --no-ansi', $projectDir);
        $generatedFile = $projectDir . '/ext/' . $project . '/export.zep.c';

        return [
            $result['exitCode'],
            $result['stderr'] . $result['stdout'],
            is_file($generatedFile) ? (string) file_get_contents($generatedFile) : '',
        ];
    }
}
