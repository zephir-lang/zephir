<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Variadic;

use function Stub\variadicSum;

final class VariadicTest extends TestCase
{
    private Variadic $test;

    protected function setUp(): void
    {
        $this->test = new Variadic();
    }

    public function testCollectNoArguments(): void
    {
        $this->assertSame([], $this->test->collect());
    }

    public function testCollectManyArguments(): void
    {
        $this->assertSame([1, 2, 3], $this->test->collect(1, 2, 3));
        $this->assertSame(['a', 'b'], $this->test->collect('a', 'b'));
    }

    public function testSumRequiredOnly(): void
    {
        $this->assertSame(1, $this->test->sum(1));
    }

    public function testSumWithVariadic(): void
    {
        $this->assertSame(6, $this->test->sum(1, 2, 3));
        $this->assertSame(15, $this->test->sum(1, 2, 3, 4, 5));
    }

    public function testCountExtra(): void
    {
        $this->assertSame(0, $this->test->countExtra('x'));
        $this->assertSame(2, $this->test->countExtra('x', 'a', 'b'));
    }

    public function testJoinTypedVariadic(): void
    {
        $this->assertSame('', $this->test->joinStrings('-'));
        $this->assertSame('a-b-c', $this->test->joinStrings('-', 'a', 'b', 'c'));
    }

    public function testOptionalFixedParamThenVariadic(): void
    {
        // [req, opt, count(params)]
        $this->assertSame([1, 0, 0], $this->test->describe(1));
        $this->assertSame([1, 2, 0], $this->test->describe(1, 2));
        $this->assertSame([1, 2, 3], $this->test->describe(1, 2, 10, 20, 30));
    }

    public function testFreeFunctionVariadic(): void
    {
        $this->assertSame(0, variadicSum());
        $this->assertSame(6, variadicSum(1, 2, 3));
        $this->assertSame(15, variadicSum(1, 2, 3, 4, 5));
    }

    public function testStaticVariadic(): void
    {
        $this->assertSame([], Variadic::staticCollect());
        $this->assertSame([1, 2, 3], Variadic::staticCollect(1, 2, 3));
    }

    public function testArrayTypedVariadic(): void
    {
        $this->assertSame(0, $this->test->totalLength());
        $this->assertSame(2, $this->test->totalLength([1, 2]));
        $this->assertSame(5, $this->test->totalLength([1, 2], [3], ['a', 'b']));
    }

    public function testObjectVariadic(): void
    {
        $this->assertSame([], $this->test->classNames());
        $this->assertSame(
            [\stdClass::class, Variadic::class],
            $this->test->classNames(new \stdClass(), new Variadic())
        );
    }

    public function testArgumentUnpacking(): void
    {
        // PHP-side spread relies on the ZEND_ARG_VARIADIC arginfo being correct.
        $args = [1, 2, 3, 4];
        $this->assertSame([1, 2, 3, 4], $this->test->collect(...$args));
        $this->assertSame(10, $this->test->sum(...$args));
    }

    public function testReflectionReportsVariadic(): void
    {
        $method = new \ReflectionMethod(Variadic::class, 'sum');
        $this->assertTrue($method->isVariadic());
        $this->assertSame(1, $method->getNumberOfRequiredParameters());

        $params = $method->getParameters();
        $this->assertTrue($params[1]->isVariadic());
        $this->assertFalse($params[0]->isVariadic());
    }

    public function testReflectionTypedVariadic(): void
    {
        $method = new \ReflectionMethod(Variadic::class, 'joinStrings');
        $params = $method->getParameters();

        $this->assertTrue($params[1]->isVariadic());
        $this->assertSame('string', (string) $params[1]->getType());
    }

    public function testReflectionFreeFunctionVariadic(): void
    {
        $function = new \ReflectionFunction('Stub\variadicSum');
        $this->assertTrue($function->isVariadic());
        $this->assertSame(0, $function->getNumberOfRequiredParameters());
    }
}
