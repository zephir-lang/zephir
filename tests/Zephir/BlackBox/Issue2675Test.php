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
 * An integer division can only be an `int` or a `float`, so it needs a zval
 * result unless the consumer is a C double: a `double` local or a `-> double`
 * return, where PHP coerces the quotient to float anyway. Those keep the
 * native helper, which is what the numeric hot loops rely on.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2675
 */
final class Issue2675Test extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Divshape;

        class Shapes
        {
            public function dynamic(long a, long b)
            {
                return a / b;
            }

            public function toDoubleLocal(long a, long b)
            {
                double d = a / b;

                return d;
            }

            public function toDoubleReturn(long a, long b) -> double
            {
                return a / b;
            }

            public function byBool(long a, bool b)
            {
                return a / b;
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

    public function testDynamicConsumerGetsAZvalResult(): void
    {
        $method = $this->method('dynamic');

        $this->assertStringContainsString('zephir_div_long_long(return_value, a, b);', $method);
        $this->assertStringNotContainsString('RETURN_MM_DOUBLE', $method);
    }

    public function testDoubleLocalKeepsTheNativeHelper(): void
    {
        $method = $this->method('toDoubleLocal');

        $this->assertStringContainsString('zephir_safe_div_long_long(a, b)', $method);
        $this->assertStringNotContainsString('zephir_div_long_long(', $method);
    }

    public function testDoubleReturnKeepsTheNativeHelper(): void
    {
        $method = $this->method('toDoubleReturn');

        $this->assertStringContainsString('zephir_safe_div_long_long(a, b)', $method);
        $this->assertStringNotContainsString('zephir_div_long_long(', $method);
    }

    public function testBoolDivisorIsADivisionNotASubtraction(): void
    {
        $method = $this->method('byBool');

        $this->assertStringContainsString('zephir_div_long_long(return_value, a, (zend_long) b);', $method);
        $this->assertStringNotContainsString('a - b', $method);
    }

    /**
     * The body of one generated method, from its PHP_METHOD line to the next.
     */
    private function method(string $name): string
    {
        $generated = $this->generate('divshape', 'shapes.zep', self::SOURCE);
        $start     = strpos($generated, 'PHP_METHOD(Divshape_Shapes, ' . $name . ')');
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
