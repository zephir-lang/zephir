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
 * Unary minus on a zval writes `operand * -1` into its own result, as PHP
 * compiles `-$x`, instead of negating the operand in place.
 *
 * The runtime behaviour is covered by tests/zept/unary_minus_parity.zept.
 */
final class UnaryMinusTest extends TestCase
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

    public function testResultIsSeparateFromTheOperand(): void
    {
        $generated = $this->generateOk(
            'umsep',
            "    public function run(a)\n    {\n        var b;\n        let b = -a;\n        return [a, b];\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_negate\(&b, a\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/',
            $generated
        );
    }

    /**
     * The operand is read, not just named: an unassigned one is still declared
     * and initialized to null (#2654), so `-x` is 0 as in PHP.
     */
    public function testUnassignedOperandIsDeclared(): void
    {
        $generated = $this->generateOk(
            'umundef',
            "    public function run()\n    {\n        var x;\n        return -x;\n    }"
        );

        // Other declarations, such as `this_ptr` on Windows before PHP 8.2, may follow `zval x;`.
        $this->assertMatchesRegularExpression('/^\s*zval x;/m', $generated);
        $this->assertMatchesRegularExpression('/ZVAL_NULL\(&x\);\s*zephir_negate\(return_value, &x\);/', $generated);
    }

    public function testNativeOperandStaysNative(): void
    {
        $generated = $this->generateOk(
            'umnat',
            "    public function run(long a) -> long\n    {\n        return -a;\n    }"
        );

        $this->assertStringContainsString('RETURN_LONG(-a);', $generated);
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
