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

namespace Extension\Oo;

class OoParamsTest extends \PHPUnit_Framework_TestCase
{
    public function testOoParams()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertTrue($t->createThisClassWithoutWriteCurrentNamespace() instanceof \Test\Oo\OoParams);
        $this->assertTrue($t->createOtherClassWithoutWriteCurrentNamespace() instanceof \Test\Oo\OoDynamicA);

        $this->assertTrue($t->setAge(17) === 17);
        $this->assertTrue($t->setAge("17") === 17);
        $this->assertTrue($t->setAge(17.0) === 17);

        $this->assertTrue($t->setAverage(17.1) === 17.1);
        $this->assertTrue($t->setAverage(17) === 17.0);
        $this->assertTrue($t->setAverage("17.5") === 17.5);

        $this->assertTrue($t->setName("peter") === "peter");
        $this->assertTrue($t->setName(12.5) === "12.5");
        $this->assertTrue($t->setName(17) === "17");

        $this->assertTrue($t->setEnabled(true) === true);
        $this->assertTrue($t->setEnabled(false) === false);
        $this->assertTrue($t->setEnabled(1) === true);
        $this->assertTrue($t->setEnabled(0) === false);
        $this->assertTrue($t->setEnabled("1") === true);
        $this->assertTrue($t->setEnabled("0") === false);

        $this->assertTrue($t->setList(array(1, 2, 3, 4, 5)) === array(1, 2, 3, 4, 5));
        $this->assertTrue($t->setList(array()) == array());
    }

    public function testSetStrictAgeException1()
    {
        $t = new \Test\Oo\OoParams();

        $this->setExpectedException('\InvalidArgumentException');
        $t->setStrictAge(17.0);
    }

    public function testSetStrictAgeException2()
    {
        $t = new \Test\Oo\OoParams();

        $this->setExpectedException('\InvalidArgumentException');
        $t->setStrictAge("17");
    }

    public function testSetStrictAgeSuccess()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertTrue($t->setStrictAverage(17.1) === 17.1);
    }

    public function testSetStrictAverageException1()
    {
        $t = new \Test\Oo\OoParams();
        $this->setExpectedException('\InvalidArgumentException');
        $t->setStrictAverage(17);
    }

    public function testSetStrictAverageException2()
    {
        $t = new \Test\Oo\OoParams();
        $this->setExpectedException('\InvalidArgumentException');
        $t->setStrictAverage("17");
    }

    public function testSetStrictNameSuccess()
    {
        $t = new \Test\Oo\OoParams();
        $this->assertTrue($t->setStrictName("peter") === "peter");
    }

    public function testSetStrickNameException()
    {
        $t = new \Test\Oo\OoParams();
        $this->setExpectedException('\InvalidArgumentException');
        $t->setStrictName(1234);
    }

    public function setObjectClassCast()
    {
        $t = new \Test\Oo\OoParams();
        $result = $t->setStrictName(new \Test\Oo\Param());

        $this->assertInstanceOf('Test\Oo\Param', $result);
    }
}
