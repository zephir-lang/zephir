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
use Stub\Fcall;
use Stub\Oo\PropertyAccess;

use function Stub\test_call_object_hint;
use function Stub\test_call_relative_object_hint;
use function Stub\zephir_namespaced_method_test;
use function Stub\zephir_namespaced_method_with_type_casting;

final class FcallTest extends TestCase
{
    private Fcall $test;

    protected function setUp(): void
    {
        $this->test = new Fcall();
    }

    public function testCall(): void
    {
        $this->assertSame(0, $this->test->testCall1());
        $this->assertGreaterThan(-1, $this->test->testCall2());
        $this->assertSame(2, $this->test->testCall1FromVar());
    }

    public function testStrtok(): void
    {
        $this->assertFalse($this->test->testStrtokFalse());
        $this->assertSame('test', $this->test->testStrtokVarBySlash('test'));
    }

    /**
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function testFunctionGetArgs($param1, $param2): void
    {
        $this->assertSame([$param1, $param2], $this->test->testFunctionGetArgs($param1, $param2));
    }

    /**
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function testShouldGetArgsUsingAllExtraParams($param1, $param2): void
    {
        $this->assertSame([$param1, $param2], $this->test->testFunctionGetArgsAllExtra($param1, $param2));
    }

    /**
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function testShouldGetArgsUsingAllExtraParamsAndStaticFunction($param1, $param2): void
    {
        $this->assertSame([$param1, $param2], Fcall::testStaticFunctionGetArgsAllExtra($param1, $param2));
    }

    public function getArgsDataProvider(): array
    {
        return [
            [true, false],
            [1025, false],
            [false, 1234],
            [[1, 2, 3], false],
            [true, false],
            [1025, false],
            [false, 1234],
        ];
    }

    public function testShouldGedDesiredArgUsingAllExtraParams(): void
    {
        $this->assertSame([true, false], $this->test->testFunctionGetArgAllExtra(true, false));
    }

    public function testShouldGedDesiredArgUsingAllExtraParamsAndStaticFunction(): void
    {
        $this->assertSame([true, false], Fcall::testStaticFunctionGetArgAllExtra(true, false));
    }

    public function testArrayFill(): void
    {
        $this->assertSame(
            [array_fill(0, 5, '?'), array_fill(0, 6, '?')],
            $this->test->testArrayFill()
        );
    }

    public function testFunctionDeclaration(): void
    {
        $this->assertSame('aaaaa', zephir_namespaced_method_test('a'));
        $this->assertTrue(test_call_relative_object_hint(new PropertyAccess()));
        $this->assertTrue(test_call_object_hint(new PropertyAccess()));

        $this->assertSame('ab', zephir_global_method_test('ab/c'));

        $this->assertInstanceOf(\stdClass::class, zephir_namespaced_method_with_type_casting(new \stdClass()));
        $this->assertInstanceOf(\stdClass::class, zephir_global_method_with_type_casting(new \stdClass()));
    }
}
