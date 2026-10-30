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
 * Compile-time facts about the bitwise operators that the runtime cannot show:
 * the width of a shifted literal, what is folded, and how a folded value is
 * spelled in C.
 *
 * The runtime behaviour is covered by tests/zept/issue2753_bitwise_parity.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2753
 */
final class Issue2753Test extends TestCase
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

    public function testShiftedIntegerLiteralIsSixtyFourBits(): void
    {
        [, $generated] = $this->generateOk(
            'i2753lit',
            "    public static function shl(int n)\n    {\n        return 1 << n;\n    }"
        );

        $this->assertStringContainsString('RETURN_LONG(zephir_safe_shift_left_long(1, n));', $generated);
    }

    /**
     * PHP does not fold an operand that loses precision as an integer; it
     * deprecates it each time the operation runs.
     */
    public function testFractionalFloatLiteralIsLeftToRuntime(): void
    {
        [$output, $generated] = $this->generateOk(
            'i2753half',
            "    public static function half()\n    {\n        return 1.5 | 1;\n    }"
        );

        $this->assertStringNotContainsString('Implicit conversion', $output);
        $this->assertStringContainsString('ZEPHIR_DVAL_TO_LVAL(1.5)', $generated);
    }

    public function testFoldedIntegerMinimumIsSpelledAsConstant(): void
    {
        [, $generated] = $this->generateOk(
            'i2753min',
            "    public static function min()\n    {\n        return 1 << 63;\n    }"
        );

        $this->assertStringContainsString('RETURN_LONG(ZEND_LONG_MIN);', $generated);
        $this->assertStringNotContainsString('9223372036854775808', $generated);
    }

    public function testDoubleOperandIsConvertedAsPhpDoes(): void
    {
        [, $generated] = $this->generateOk(
            'i2753dbl',
            "    public static function mask(long a, double d)\n    {\n        return a | d;\n    }"
        );

        $this->assertStringContainsString('ZEPHIR_DVAL_TO_LVAL(d)', $generated);
        $this->assertStringNotContainsString('(int) (d)', $generated);
    }

    /**
     * @return array{0: string, 1: string} the compiler output and the generated C
     */
    private function generateOk(string $name, string $body): array
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

        return [$result['stdout'] . $result['stderr'], (string) file_get_contents($generatedFile)];
    }
}
