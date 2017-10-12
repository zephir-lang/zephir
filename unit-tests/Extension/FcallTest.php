<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class FcallTest extends \PHPUnit_Framework_TestCase
{
    public function testCall()
    {
        $t = new \Test\Fcall();
        $this->assertSame($t->testCall1(), 0);
        $this->assertLessThan($t->testCall2(), -1);
        $this->assertSame($t->testCall1FromVar(), 2);
    }

    public function testStrtok()
    {
        $t = new \Test\Fcall();
        $this->assertFalse($t->testStrtokFalse());
        $this->assertSame($t->testStrtokVarBySlash('test'), 'test');
    }

    public function testFunctionGetArgs()
    {
        $t = new \Test\Fcall();
        $this->assertSame($t->testFunctionGetArgs(true, false), array(true, false));
        $this->assertSame($t->testFunctionGetArgs(1025, false), array(1025, false));
        $this->assertSame($t->testFunctionGetArgs(false, 1234), array(false, 1234));
        $this->assertSame($t->testFunctionGetArgs(array(1, 2, 3), false), array(array(1, 2, 3), false));

        $this->assertSame($t->testFunctionGetArg(true, false), array(true, false));
        $this->assertSame($t->testFunctionGetArg(1025, false), array(1025, false));
        $this->assertSame($t->testFunctionGetArg(false, 1234), array(false, 1234));
    }

    public function testArrayFill()
    {
        $t = new \Test\Fcall();
        $this->assertSame($t->testArrayFill(), array(array_fill(0, 5, '?'), array_fill(0, 6, '?')));
    }

    public function testFunctionDeclaration()
    {
        $this->assertSame("aaaaa", \Test\zephir_namespaced_method_test("a"));
        $this->assertSame("ab", zephir_global_method_test("ab/c"));

        $this->assertInstanceOf(\stdClass::class, \Test\zephir_namespaced_method_with_type_casting(new \stdClass()));
        $this->assertInstanceOf(\stdClass::class, zephir_global_method_with_type_casting(new \stdClass()));
    }
}
