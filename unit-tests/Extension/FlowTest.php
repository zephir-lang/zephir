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

class FlowTest extends \PHPUnit_Framework_TestCase
{
    public function testIf()
    {
        $t = new \Test\Flow();

        $this->assertTrue($t->testIf1() === 1);
        $this->assertTrue($t->testIf2() === 0);
        $this->assertTrue($t->testIf3() === 1);
        $this->assertTrue($t->testIf4() === 0);
        $this->assertTrue($t->testIf5() === 1);
        $this->assertTrue($t->testIf6() === 0);
        $this->assertTrue($t->testIf7() === 1);
        $this->assertTrue($t->testIf8() === 0);
        $this->assertTrue($t->testIf9() === 1);
        $this->assertTrue($t->testIf10() === 654);
        $this->assertTrue($t->testIf12() === 987);
        $this->assertTrue($t->testIf13() === -12);
        $this->assertTrue($t->testIf14() === 74);
        $this->assertTrue($t->testIf15() === 89);
        $this->assertTrue($t->testIf16(array()) === true);
        $this->assertTrue($t->testIf16('') === true);
        $this->assertTrue($t->testIf16(null) === true);
        $this->assertTrue($t->testIf16(' ') === false);
    }

    public function testLoop()
    {
        $t = new \Test\Flow();
        $this->assertTrue($t->testLoop1() === true);
        $this->assertTrue($t->testLoop2() === 5);
        $this->assertTrue($t->testLoop3() === 5);
    }

    public function testWhile()
    {
        $t = new \Test\Flow();
        $this->assertTrue($t->testWhile1() === 0);
        $this->assertTrue($t->testWhile2() === 0);
        $this->assertTrue($t->testWhile3() == 0.0);
        $this->assertTrue($t->testWhile4() == 0.0);
        $this->assertTrue($t->testWhile5() == 0.0);
        $this->assertTrue($t->testWhile6() == 0.0);
        $this->assertTrue($t->testWhile7() == 0.0);
        $this->assertTrue($t->testWhile8() == 0.0);
        $this->assertTrue($t->testWhile9() == 0.0);
        $this->assertTrue($t->testWhile12() == 5);
        $this->assertTrue($t->testWhile13() == 5);

        $this->assertTrue($t->testWhileNextTest(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) == array(1, 2, 3, 4, 5, 6, 7, 8, 9));
        $this->assertTrue($t->testWhileDoNextTest(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) == array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
    }

    public function testFor()
    {
        $t = new \Test\Flow();
        $this->assertTrue($t->testFor1() === 10);
        $this->assertTrue($t->testFor2() === 6.0);
        $this->assertTrue($t->testFor3() === array(4, 3, 2, 1));
        $this->assertTrue($t->testFor4() === 55);
        $this->assertTrue($t->testFor5() === 55);
        $this->assertTrue($t->testFor6() === 55);
        $this->assertTrue($t->testFor7() === 55);
        $this->assertTrue($t->testFor8() === 55);
        $this->assertTrue($t->testFor9() === 55);
        $this->assertTrue($t->testFor10() === 55);
        $this->assertTrue($t->testFor11() === 'abcdefghijklmnopqrstuvwxyz');
        $this->assertTrue($t->testFor12() === 'zyxwvutsrqponmlkjihgfedcba');
        $this->assertTrue($t->testFor13() === '0123456789');
        $this->assertTrue($t->testFor14() === '9876543210');
        $this->assertTrue($t->testFor15(1, 10) === 55);

        $this->assertTrue($t->testFor16() === array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10));
        $this->assertTrue($t->testFor17() === array(0 => 10, 1 => 9, 2 => 8, 3 => 7, 4 => 6, 5 => 5, 6 => 4, 7 => 3, 8 => 2, 9 => 1));
        $this->assertTrue($t->testFor18() === array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10));

        $this->assertTrue($t->testFor19() === 25);
        $this->assertTrue($t->testFor20() === 25);
        $this->assertTrue($t->testFor21() === 0);
        $this->assertTrue($t->testFor22() === 0);
        $this->assertTrue($t->testFor23() === "zxvtrpnljhfdb");
    }

    public function testSwitch()
    {
        $t = new \Test\Flow();
        $this->assertEquals($t->testSwitch1(), 10);
        $this->assertEquals($t->testSwitch2(), true);
        $this->assertEquals($t->testSwitch3(), false);
        $this->assertEquals($t->testSwitch4(), 2);
        $this->assertEquals($t->testSwitch5(), 1);
        $this->assertEquals($t->testSwitch6(), 1);
        $this->assertEquals($t->testSwitch7(), 1);
        $this->assertEquals($t->testSwitch8(), 2);
        $this->assertEquals($t->testSwitch9(), 2);
        $this->assertEquals($t->testSwitch10(), 2);
        $this->assertEquals($t->testSwitch11(), 1);
        $this->assertEquals($t->testSwitch12(10, 6), 1);
        $this->assertEquals($t->testSwitch12(6, 10), 0);
        $this->assertEquals($t->testSwitch12(2, 3), 2);
    }
}
