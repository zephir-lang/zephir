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
 * A zval combined with a native number by `+`, `-` or `*` can hold a float,
 * a numeric string or something PHP rejects, so the result must come from
 * PHP's own operator. Reading it as a C number and typing the result `int`
 * truncates floats. Two native operands keep plain C arithmetic.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2744
 */
final class Issue2744Test extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Arithshape;

        class Shapes
        {
            public function varPlusLiteral(var a)
            {
                return a + 1;
            }

            public function longMinusVar(long b, var a)
            {
                return b - a;
            }

            public function doubleTimesVar(double d, var a)
            {
                return d * a;
            }

            public function boolPlusVar(bool b, var a)
            {
                return b + a;
            }

            public function charPlusVar(var a)
            {
                char c = 'a';

                return c + a;
            }

            public function varPlusLong(var a, long b)
            {
                return a + b;
            }

            public function arrayPlusLiteral(array a)
            {
                return a + 1;
            }

            public function longPlusLong(long a, long b)
            {
                return a + b;
            }

            public function doublePlusLong(double a, long b)
            {
                return a + b;
            }
        }
        ZEP;

    private static ?string $generated = null;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public static function zvalShapeProvider(): array
    {
        return [
            'var + literal'  => ['varPlusLiteral', 'zephir_add_function('],
            'long - var'     => ['longMinusVar', 'zephir_sub_function('],
            'double * var'   => ['doubleTimesVar', 'mul_function('],
            'bool + var'     => ['boolPlusVar', 'zephir_add_function('],
            'char + var'     => ['charPlusVar', 'zephir_add_function('],
            'var + long'     => ['varPlusLong', 'zephir_add_function('],
            'array + int'    => ['arrayPlusLiteral', 'zephir_add_function('],
        ];
    }

    /**
     * @dataProvider zvalShapeProvider
     */
    public function testZvalWithNativeNumberUsesTheZvalOperator(string $name, string $operator): void
    {
        $method = $this->method($name);

        $this->assertStringContainsString($operator, $method);
        $this->assertStringNotContainsString('zephir_get_numberval', $method);
    }

    public function testTwoLongsKeepCArithmetic(): void
    {
        $method = $this->method('longPlusLong');

        $this->assertStringContainsString('RETURN_LONG((a + b));', $method);
        $this->assertStringNotContainsString('zephir_add_function(', $method);
    }

    public function testDoubleWithLongKeepsCArithmetic(): void
    {
        $method = $this->method('doublePlusLong');

        $this->assertStringContainsString('RETURN_DOUBLE((a + (double) (b)));', $method);
        $this->assertStringNotContainsString('zephir_add_function(', $method);
    }

    /**
     * The body of one generated method, from its PHP_METHOD line to the next.
     */
    private function method(string $name): string
    {
        self::$generated ??= $this->generate('arithshape', 'shapes.zep', self::SOURCE);
        $start = strpos(self::$generated, 'PHP_METHOD(Arithshape_Shapes, ' . $name . ')');
        $this->assertNotFalse($start, 'PHP_METHOD for ' . $name . ' not generated');
        $end = strpos(self::$generated, 'PHP_METHOD(', $start + 1);

        return false === $end ? substr(self::$generated, $start) : substr(self::$generated, $start, $end - $start);
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
