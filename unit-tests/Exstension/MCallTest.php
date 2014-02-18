<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Exstension;

class MCallTest
    extends \PHPUnit_Framework_TestCase
{
    public function testCall()
    {
        $t = new \Test\Mcall();

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
        $t = new \Test\Mcall();
        $this->assertTrue($t->optionalParameterString("test") == 'test');
        $this->assertTrue($t->optionalParameterString() == "test string");
        $this->assertTrue($t->optionalParameterStringNull() == "");
    }

    public function testOptionalParameterInt()
    {
        $t = new \Test\Mcall();
        $this->assertTrue($t->optionalParameterInt(1) == 1);
        $this->assertTrue($t->optionalParameterInt() == 2);
    }

    public function testOptionalParameterVar()
    {
        $t = new \Test\Mcall();
        $this->assertTrue($t->optionalParameterVar(1) === 1);
        $this->assertTrue($t->optionalParameterVar("testtesttesttest") === "testtesttesttest");
        $this->assertTrue($t->optionalParameterVar() === NULL);
    }

    public function testOptionalParameterBoolean()
    {
        $t = new \Test\Mcall();
        $this->assertTrue($t->optionalParameterBoolFalse() === false);
        $this->assertTrue($t->optionalParameterBoolTrue() === true);

        $this->assertTrue($t->optionalParameterBooleanNull() == false);
        $this->assertTrue($t->optionalParameterBooleanNull(true) == true);

        $this->assertTrue($t->optionalParameterBoolean() === true);
        $this->assertTrue($t->optionalParameterBoolean(false) === false);
    }

    public function testOptionalParameterBooleanException()
    {
        $t = new \Test\Mcall();
        $this->setExpectedException('\InvalidArgumentException');
        $t->optionalParameterBoolean('test');
    }

    public function testArrayParamWithDefaultEmptyArray()
    {
        $t = new \Test\Mcall();
        $this->assertTrue($t->arrayParamWithDefaultEmptyArray() === array());
        $this->assertTrue($t->arrayParamWithDefaultEmptyArray(array(1)) === array(1));
    }
} 