<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\Mcall;
use stdClass;

class MCallTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @var \ReflectionClass
     */
    private $reflection;

    public function assertNumberOfParameters($number)
    {
        $this->assertEquals($number, $this->getReflection()->getMethod($this->getName())->getNumberOfParameters());
    }

    public function assertNumberOfRequiredParameters($number)
    {
        $this->assertEquals($number, $this->getReflection()->getMethod($this->getName())->getNumberOfRequiredParameters());
    }

    private function getReflection()
    {
        if (is_null($this->reflection)) {
            return $this->reflection = new \ReflectionClass('\Test\Mcall');
        }

        return $this->reflection;
    }

    /**
     * @return \ReflectionParameter
     */
    protected function getMethodFirstParameter()
    {
        $backtrace = debug_backtrace();
        $methodInfo = $this->reflection->getMethod($this->getName());
        $parameters = $methodInfo->getParameters();

        return $parameters[0];
    }

    public function testCall()
    {
        $t = new Mcall();

        $this->assertTrue($t->testCall1() === "hello public");
        $this->assertTrue($t->testCall2() === "hello protected");
        $this->assertTrue($t->testCall3() === "hello private");

        $this->assertTrue($t->testCall4(2, 3) === 5);
        $this->assertTrue($t->testCall5(6, 7) === 13);
        $this->assertTrue($t->testCall6(4, 5) === 9);

        $this->assertTrue($t->testCall7(2, 3) === 10);
        $this->assertTrue($t->testCall8(6, 7) === 26);
        $this->assertTrue($t->testCall9(4, 5) === 18);

        $this->assertTrue($t->testCall10() === "hello public");
        $this->assertTrue($t->testCall11() === "hello protected");
        $this->assertTrue($t->testCall12() === "hello private");

        $this->assertTrue($t->testCall13(2, 3) === 5);
        $this->assertTrue($t->testCall14(6, 7) === 13);
        $this->assertTrue($t->testCall15(4, 5) === 9);
    }

    public function testOptionalParameterString()
    {
        $t = new Mcall();
        $this->assertTrue($t->optionalParameterString("test") == 'test');
        $this->assertTrue($t->optionalParameterString() == "test string");
        $this->assertTrue($t->optionalParameterStringNull() == "");
    }

    public function testOptionalParameterInt()
    {
        $t = new Mcall();
        $this->assertTrue($t->optionalParameterInt(1) == 1);
        $this->assertTrue($t->optionalParameterInt() == 2);
    }

    public function testOptionalParameterVar()
    {
        $t = new Mcall();
        $this->assertTrue($t->optionalParameterVar(1) === 1);
        $this->assertTrue($t->optionalParameterVar("testtesttesttest") === "testtesttesttest");
        $this->assertTrue($t->optionalParameterVar() === null);
    }

    public function testOptionalParameterBoolean()
    {
        $t = new Mcall();
        $this->assertTrue($t->optionalParameterBoolFalse() === false);
        $this->assertTrue($t->optionalParameterBoolTrue() === true);

        $this->assertTrue($t->optionalParameterBooleanNull() == false);
        $this->assertTrue($t->optionalParameterBooleanNull(true) == true);

        $this->assertTrue($t->optionalParameterBoolean() === true);
        $this->assertTrue($t->optionalParameterBoolean(false) === false);
    }

    public function testOptionalParameterBooleanException()
    {
        $t = new Mcall();
        $this->setExpectedException('\InvalidArgumentException');
        $t->optionalParameterBoolean('test');
    }

    /**
     * @test
     */
    public function arrayParamWithDefaultEmptyArray()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertTrue($t->arrayParamWithDefaultEmptyArray() === array());
        $this->assertTrue($t->arrayParamWithDefaultEmptyArray(array(1)) === array(1));
    }

    /**
     * @test
     */
    public function arrayParamWithDefaultNullValue()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertTrue($t->arrayParamWithDefaultNullValue() === array());
        $this->assertTrue($t->arrayParamWithDefaultNullValue(array(1)) === array(1));
    }

    /**
     * @test
     */
    public function arrayParam()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertTrue($t->arrayParam(array()) === array());
        $this->assertTrue($t->arrayParam(array(1, 2, 3)) === array(1, 2, 3));
    }

    /**
     * @test
     */
    public function objectParamCastStdClass()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('stdClass', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('stdClass', $t->objectParamCastStdClass(new \stdClass()));
    }

    /**
     * @test
     */
    public function objectParamCastOoParam()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('Test\Oo\Param', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('Test\Oo\Param', $t->objectParamCastOoParam(new \Test\Oo\Param()));
    }
}
