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

namespace Extension\Oo;

class OoParamsTest extends \PHPUnit_Framework_TestCase
{
    public function testOoParams()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertInstanceOf('Test\Oo\OoParams', $t->createThisClassWithoutWriteCurrentNamespace());
        $this->assertinstanceOf('Test\Oo\OoDynamicA', $t->createOtherClassWithoutWriteCurrentNamespace());

        $this->assertSame($t->setAge(17), 17);
        $this->assertSame($t->setAge('17'), 17);
        $this->assertSame($t->setAge(17.0), 17);

        $this->assertSame($t->setAverage(17.1), 17.1);
        $this->assertSame($t->setAverage(17), 17.0);
        $this->assertSame($t->setAverage('17.5'), 17.5);

        $this->assertSame($t->setName('peter'), 'peter');
        $this->assertSame($t->setName(12.5), '12.5');
        $this->assertSame($t->setName(17), '17');

        $this->assertSame($t->setEnabled(true), true);
        $this->assertSame($t->setEnabled(false), false);
        $this->assertSame($t->setEnabled(1), true);
        $this->assertSame($t->setEnabled(0), false);
        $this->assertSame($t->setEnabled('1'), true);
        $this->assertSame($t->setEnabled('0'), false);

        $this->assertSame($t->setList(array(1, 2, 3, 4, 5)), array(1, 2, 3, 4, 5));
        $this->assertSame($t->setList(array()), array());
    }

    public function testSetStrictAgeException1()
    {
        $t = new \Test\Oo\OoParams();

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\InvalidArgumentException');
        } else {
            $this->expectException('\InvalidArgumentException');
        }

        $t->setStrictAge(17.0);
    }

    public function testSetStrictAgeException2()
    {
        $t = new \Test\Oo\OoParams();

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\InvalidArgumentException');
        } else {
            $this->expectException('\InvalidArgumentException');
        }

        $t->setStrictAge('17');
    }

    public function testSetStrictAgeSuccess()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertSame($t->setStrictAverage(17.1), 17.1);
    }

    public function testSetStrictAverageException1()
    {
        $t = new \Test\Oo\OoParams();

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\InvalidArgumentException');
        } else {
            $this->expectException('\InvalidArgumentException');
        }

        $t->setStrictAverage(17);
    }

    public function testSetStrictAverageException2()
    {
        $t = new \Test\Oo\OoParams();

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\InvalidArgumentException');
        } else {
            $this->expectException('\InvalidArgumentException');
        }

        $t->setStrictAverage('17');
    }

    public function testSetStrictNameSuccess()
    {
        $t = new \Test\Oo\OoParams();
        $this->assertSame($t->setStrictName('peter'), 'peter');
    }

    public function testSetStrickNameException()
    {
        $t = new \Test\Oo\OoParams();

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\InvalidArgumentException');
        } else {
            $this->expectException('\InvalidArgumentException');
        }

        $t->setStrictName(1234);
    }

    public function setObjectClassCast()
    {
        $t = new \Test\Oo\OoParams();
        $result = $t->setStrictName(new \Test\Oo\Param());

        $this->assertInstanceOf('Test\Oo\Param', $result);
    }
}
