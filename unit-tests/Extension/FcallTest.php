<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Fcall;
use Test\Oo\PropertyAccess;

class UserExample extends \Test\Oo\PropertyAccess {}

class FcallTest extends TestCase
{
    public function testCall()
    {
        $t = new Fcall();
        $this->assertSame(0, $t->testCall1());
        $this->assertGreaterThan(-1, $t->testCall2());
        $this->assertSame(2, $t->testCall1FromVar());
    }

    public function testStrtok()
    {
        $t = new Fcall();
        $this->assertFalse($t->testStrtokFalse());
        $this->assertSame('test', $t->testStrtokVarBySlash('test'));
    }

    /**
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function testFunctionGetArgs($param1, $param2)
    {
        $t = new Fcall();
        $this->assertSame([$param1, $param2], $t->testFunctionGetArgs($param1, $param2));
    }

    /**
     * @test
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function shouldGetArgsUsingAllExtraParams($param1, $param2)
    {
        $t = new Fcall();
        $this->assertSame([$param1, $param2], $t->testFunctionGetArgsAllExtra($param1, $param2));
    }

    /**
     * @test
     * @dataProvider getArgsDataProvider
     *
     * @param mixed $param1
     * @param mixed $param2
     */
    public function shouldGetArgsUsingAllExtraParamsAndStaticFunction($param1, $param2)
    {
        $this->assertSame([$param1, $param2], Fcall::testStaticFunctionGetArgsAllExtra($param1, $param2));
    }

    public function getArgsDataProvider()
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

    /** @test */
    public function shouldGedDesiredArgUsingAllExtraParams()
    {
        $t = new Fcall();
        $this->assertSame([true, false], $t->testFunctionGetArgAllExtra(true, false));
    }

    /** @test */
    public function shouldGedDesiredArgUsingAllExtraParamsAndStaticFunction()
    {
        $this->assertSame([true, false], Fcall::testStaticFunctionGetArgAllExtra(true, false));
    }

    public function testArrayFill()
    {
        $t = new Fcall();
        $this->assertSame(
            [array_fill(0, 5, '?'), array_fill(0, 6, '?')],
            $t->testArrayFill()
        );
    }

    public function testFunctionDeclaration()
    {
        $this->assertSame('aaaaa', \Test\zephir_namespaced_method_test('a'));
        $this->assertTrue(\Test\test_call_relative_object_hint(new PropertyAccess()));
        $this->assertTrue(\Test\test_call_object_hint(new PropertyAccess()));

        $this->assertSame('ab', zephir_global_method_test('ab/c'));

        $this->assertInstanceOf(\stdClass::class, \Test\zephir_namespaced_method_with_type_casting(new \stdClass()));
        $this->assertInstanceOf(\stdClass::class, zephir_global_method_with_type_casting(new \stdClass()));
    }

    public function testIssue1851()
    {
        $example = new UserExample;
        $example->setPrivateVariable('test');

        $this->assertSame('test', $example->getPrivateVariable());
    }
}
