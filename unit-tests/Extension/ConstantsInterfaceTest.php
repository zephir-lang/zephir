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

class ConstantsInterfaceTest extends \PHPUnit_Framework_TestCase
{
    public function testConstants()
    {
        assert(\Test\TestInterface::C1 === null);
        assert(\Test\TestInterface::C2 === false);
        assert(\Test\TestInterface::C3 === true);
        assert(\Test\TestInterface::C4 === 10);
        assert(\Test\TestInterface::C5 === 10.25);
        assert(\Test\TestInterface::C6 === "test");
    }

    public function testClassOfInterface()
    {
        $t = new \Test\ConstantsInterface();

        assert(is_object($t));
        assert($t->testReadInterfaceConstant1() === null);
        assert($t->testReadInterfaceConstant2() === false);
        assert($t->testReadInterfaceConstant3() === true);
        assert($t->testReadInterfaceConstant4() === 10);
        assert($t->testReadInterfaceConstant5() === 10.25);
        assert($t->testReadInterfaceConstant6() === "test");
    }
}
