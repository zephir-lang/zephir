<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class FlowTest extends TestCase
{
    /** @var \Test\Flow */
    private $test;

    public function setUp()
    {
        $this->test = new \Test\Flow();
    }

    public function testIf()
    {
        $this->assertSame($this->test->testIf1(), 1);
        $this->assertSame($this->test->testIf2(), 0);
        $this->assertSame($this->test->testIf3(), 1);
        $this->assertSame($this->test->testIf4(), 0);
        $this->assertSame($this->test->testIf5(), 1);
        $this->assertSame($this->test->testIf6(), 0);
        $this->assertSame($this->test->testIf7(), 1);
        $this->assertSame($this->test->testIf8(), 0);
        $this->assertSame($this->test->testIf9(), 1);
        $this->assertSame($this->test->testIf10(), 654);
        $this->assertSame($this->test->testIf12(), 987);
        $this->assertSame($this->test->testIf13(), -12);
        $this->assertSame($this->test->testIf14(), 74);
        $this->assertSame($this->test->testIf15(), 89);
        $this->assertTrue($this->test->testIf16([]));
        $this->assertTrue($this->test->testIf16(''));
        $this->assertTrue($this->test->testIf16(null));
        $this->assertFalse($this->test->testIf16(' '));
    }

    public function testLoop()
    {
        $this->assertTrue($this->test->testLoop1());
        $this->assertSame($this->test->testLoop2(), 5);
        $this->assertSame($this->test->testLoop3(), 5);
    }

    public function testWhile()
    {
        $this->assertSame($this->test->testWhile1(), 0);
        $this->assertSame($this->test->testWhile2(), 0);
        $this->assertSame($this->test->testWhile3(), 0.0);
        $this->assertSame($this->test->testWhile4(), 0.0);
        $this->assertSame($this->test->testWhile5(), 0);
        $this->assertSame($this->test->testWhile6(), 0);
        $this->assertSame($this->test->testWhile7(), 0.0);
        $this->assertSame($this->test->testWhile8(), 0.0);
        $this->assertSame($this->test->testWhile9(), 0.0);
        $this->assertSame($this->test->testWhile10(10), 0.0);
        $this->assertSame($this->test->testWhile11(1, 10), 0.0);
        $this->assertSame($this->test->testWhile12(), 5);
        $this->assertSame($this->test->testWhile13(), 5);

        $this->assertSame(
            $this->test->testWhileNextTest([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
            [1, 2, 3, 4, 5, 6, 7, 8, 9]
        );

        $this->assertSame(
            $this->test->testWhileDoNextTest([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]),
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
        );
    }

    public function testFor()
    {
        $this->assertSame($this->test->testFor1(), 10);
        $this->assertSame($this->test->testFor2(), 6.0);
        $this->assertSame($this->test->testFor3(), [4, 3, 2, 1]);
        $this->assertSame($this->test->testFor4(), 55);
        $this->assertSame($this->test->testFor5(), 55);
        $this->assertSame($this->test->testFor6(), 55);
        $this->assertSame($this->test->testFor7(), 55);
        $this->assertSame($this->test->testFor8(), 55);
        $this->assertSame($this->test->testFor9(), 55);
        $this->assertSame($this->test->testFor10(), 55);
        $this->assertSame($this->test->testFor11(), 'abcdefghijklmnopqrstuvwxyz');
        $this->assertSame($this->test->testFor12(), 'zyxwvutsrqponmlkjihgfedcba');
        $this->assertSame($this->test->testFor13(), '0123456789');
        $this->assertSame($this->test->testFor14(), '9876543210');
        $this->assertSame($this->test->testFor15(1, 10), 55);

        $this->assertSame(
            $this->test->testFor16(),
            [0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10]
        );

        $this->assertSame(
            $this->test->testFor17(),
            [0 => 10, 1 => 9, 2 => 8, 3 => 7, 4 => 6, 5 => 5, 6 => 4, 7 => 3, 8 => 2, 9 => 1]
        );

        $this->assertSame(
            $this->test->testFor18(),
            [0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10]
        );

        $this->assertSame($this->test->testFor19(), 25);
        $this->assertSame($this->test->testFor20(), 25);
        $this->assertSame($this->test->testFor21(), 0);
        $this->assertSame($this->test->testFor22(), 0);
        $this->assertSame($this->test->testFor23(), 'zxvtrpnljhfdb');
    }
}
