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
 * `<<` and `>>` follow PHP rather than C: two zvals go through the engine, and
 * C integers through helpers that handle a negative or too-wide count, unless
 * the count is a literal C defines.
 *
 * The runtime behaviour is covered by tests/zept/shift_operators_parity.zept.
 */
final class ShiftOperatorTest extends TestCase
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

    public function testTwoZvalsGoThroughTheEngine(): void
    {
        $generated = $this->generateOk(
            'shzv',
            "    public function run(a, b)\n    {\n        var r;\n        let r = a << b;\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_shift_left_function\(&r, a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/',
            $generated
        );
    }

    public function testLiteralCountInRangeStaysInline(): void
    {
        $generated = $this->generateOk(
            'shlit',
            "    public function run(long a) -> long\n    {\n        return a << 3;\n    }"
        );

        $this->assertStringContainsString('RETURN_LONG(((zend_long) ((zend_ulong) (a) << 3)));', $generated);
    }

    public function testVariableCountIsCheckedThroughTheHelper(): void
    {
        $generated = $this->generateOk(
            'shvar',
            "    public function run(long a, long b) -> long\n    {\n        long r;\n        let r = a >> b;\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/= zephir_safe_shift_right_long\(a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/',
            $generated
        );
    }

    /**
     * A folded literal reaches the outer operator as an int, not as C code.
     */
    public function testFoldedOperandInsideAnotherOperation(): void
    {
        $generated = $this->generateOk(
            'shnest',
            "    public function run(long a) -> long\n    {\n        return (1 << 2) & a | (a << (8 - 3));\n    }"
        );

        $this->assertStringContainsString('4 & a', $generated);
    }

    public function testNegativeLiteralCountIsNotFolded(): void
    {
        $generated = $this->generateOk(
            'shfold',
            "    public function run()\n    {\n        return 1 << -1;\n    }"
        );

        $this->assertStringContainsString('zephir_safe_shift_left_long(1, -1)', $generated);
    }

    private function generateOk(string $name, string $body): string
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf('%s/ext/%s/sample.zep.c', $projectDir, $name);
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }
}
