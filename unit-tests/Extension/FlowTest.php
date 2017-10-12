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

class FlowTest extends \PHPUnit_Framework_TestCase
{
    public function testIf()
    {
        $t = new \Test\Flow();

        $this->assertSame($t->testIf1(), 1);
        $this->assertSame($t->testIf2(), 0);
        $this->assertSame($t->testIf3(), 1);
        $this->assertSame($t->testIf4(), 0);
        $this->assertSame($t->testIf5(), 1);
        $this->assertSame($t->testIf6(), 0);
        $this->assertSame($t->testIf7(), 1);
        $this->assertSame($t->testIf8(), 0);
        $this->assertSame($t->testIf9(), 1);
        $this->assertSame($t->testIf10(), 654);
        $this->assertSame($t->testIf12(), 987);
        $this->assertSame($t->testIf13(), -12);
        $this->assertSame($t->testIf14(), 74);
        $this->assertSame($t->testIf15(), 89);
        $this->assertTrue($t->testIf16(array()));
        $this->assertTrue($t->testIf16(''));
        $this->assertTrue($t->testIf16(null));
        $this->assertFalse($t->testIf16(' '));
    }

    public function testLoop()
    {
        $t = new \Test\Flow();
        $this->assertTrue($t->testLoop1());
        $this->assertSame($t->testLoop2(), 5);
        $this->assertSame($t->testLoop3(), 5);
    }

    public function testWhile()
    {
        $t = new \Test\Flow();
        $this->assertSame($t->testWhile1(), 0);
        $this->assertSame($t->testWhile2(), 0);
        $this->assertSame($t->testWhile3(), 0.0);
        $this->assertSame($t->testWhile4(), 0.0);
        $this->assertSame($t->testWhile5(), 0);
        $this->assertSame($t->testWhile6(), 0);
        $this->assertSame($t->testWhile7(), 0.0);
        $this->assertSame($t->testWhile8(), 0.0);
        $this->assertSame($t->testWhile9(), 0.0);
        $this->assertSame($t->testWhile10(10), 0.0);
        $this->assertSame($t->testWhile11(1, 10), 0.0);
        $this->assertSame($t->testWhile12(), 5);
        $this->assertSame($t->testWhile13(), 5);

        $this->assertSame(
            $t->testWhileNextTest(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)),
            array(1, 2, 3, 4, 5, 6, 7, 8, 9)
        );

        $this->assertSame(
            $t->testWhileDoNextTest(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)),
            array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
        );
    }

    public function testFor()
    {
        $t = new \Test\Flow();
        $this->assertSame($t->testFor1(), 10);
        $this->assertSame($t->testFor2(), 6.0);
        $this->assertSame($t->testFor3(), array(4, 3, 2, 1));
        $this->assertSame($t->testFor4(), 55);
        $this->assertSame($t->testFor5(), 55);
        $this->assertSame($t->testFor6(), 55);
        $this->assertSame($t->testFor7(), 55);
        $this->assertSame($t->testFor8(), 55);
        $this->assertSame($t->testFor9(), 55);
        $this->assertSame($t->testFor10(), 55);
        $this->assertSame($t->testFor11(), 'abcdefghijklmnopqrstuvwxyz');
        $this->assertSame($t->testFor12(), 'zyxwvutsrqponmlkjihgfedcba');
        $this->assertSame($t->testFor13(), '0123456789');
        $this->assertSame($t->testFor14(), '9876543210');
        $this->assertSame($t->testFor15(1, 10), 55);

        $this->assertSame(
            $t->testFor16(),
            array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10)
        );

        $this->assertSame(
            $t->testFor17(),
            array(0 => 10, 1 => 9, 2 => 8, 3 => 7, 4 => 6, 5 => 5, 6 => 4, 7 => 3, 8 => 2, 9 => 1)
        );

        $this->assertSame(
            $t->testFor18(),
            array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10)
        );

        $this->assertSame($t->testFor19(), 25);
        $this->assertSame($t->testFor20(), 25);
        $this->assertSame($t->testFor21(), 0);
        $this->assertSame($t->testFor22(), 0);
        $this->assertSame($t->testFor23(), 'zxvtrpnljhfdb');
    }

    public function testSwitch()
    {
        $t = new \Test\Flow();
        $this->assertSame($t->testSwitch1(), 10);
        $this->assertTrue($t->testSwitch2());
        $this->assertFalse($t->testSwitch3());
        $this->assertSame($t->testSwitch4(), 2);
        $this->assertSame($t->testSwitch5(), 1);
        $this->assertSame($t->testSwitch6(), 1);
        $this->assertSame($t->testSwitch7(), 1);
        $this->assertSame($t->testSwitch8(), 2);
        $this->assertSame($t->testSwitch9(), 2);
        $this->assertSame($t->testSwitch10(), 2);
        $this->assertSame($t->testSwitch11(), 1);
        $this->assertSame($t->testSwitch12(10, 6), 1);
        $this->assertSame($t->testSwitch12(6, 10), 0);
        $this->assertSame($t->testSwitch12(2, 3), 2);
    }
}
