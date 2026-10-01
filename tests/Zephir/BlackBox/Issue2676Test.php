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
 * Two native operands keep the native `%` helper, typed `int`, which is what
 * the numeric hot loops rely on. A zval operand goes through PHP's own
 * mod_function(), and `%=` is rewritten as `x = x % e` so the zero and `-1`
 * divisor guards run for every target.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2676
 * @see https://github.com/zephir-lang/zephir/issues/2677
 */
final class Issue2676Test extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Modshape;

        class Shapes
        {
            public function native(long a, long b) -> long
            {
                return a % b;
            }

            public function zvalDividend(var a, long b)
            {
                return a % b;
            }

            public function byBool(long a, bool b)
            {
                return a % b;
            }

            public function assignTyped(long a, long b)
            {
                long x;

                let x = a;
                let x %= b;

                return x;
            }

            public function divAssignTyped(long a, long b)
            {
                long x;

                let x = a;
                let x /= b;

                return x;
            }
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testNativeOperandsKeepTheNativeHelper(): void
    {
        $method = $this->method('native');

        $this->assertStringContainsString('RETURN_LONG(zephir_safe_mod_long_long(a, b));', $method);
    }

    public function testZvalOperandGoesThroughModFunction(): void
    {
        $method = $this->method('zvalDividend');

        $this->assertStringContainsString('zephir_mod_zval_long(return_value, a, b);', $method);
    }

    public function testBoolDivisorIsAModuloNotASubtraction(): void
    {
        $method = $this->method('byBool');

        $this->assertStringContainsString('zephir_safe_mod_long_long(a, (zend_long) b)', $method);
        $this->assertStringNotContainsString('a - b', $method);
    }

    public function testModAssignOnATypedLocalKeepsTheGuards(): void
    {
        $method = $this->method('assignTyped');

        $this->assertStringContainsString('zephir_safe_mod_long_long(x, b)', $method);
        $this->assertStringNotContainsString('%=', $method);
    }

    public function testDivAssignOnATypedLocalKeepsTheGuards(): void
    {
        $method = $this->method('divAssignTyped');

        $this->assertStringNotContainsString('/=', $method);
    }

    /**
     * The body of one generated method, from its PHP_METHOD line to the next.
     */
    private function method(string $name): string
    {
        $generated = $this->generate('modshape', 'shapes.zep', self::SOURCE);
        $start     = strpos($generated, 'PHP_METHOD(Modshape_Shapes, ' . $name . ')');
        $this->assertNotFalse($start, 'PHP_METHOD for ' . $name . ' not generated');
        $end = strpos($generated, 'PHP_METHOD(', $start + 1);

        return false === $end ? substr($generated, $start) : substr($generated, $start, $end - $start);
    }

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
