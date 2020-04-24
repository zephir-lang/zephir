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

class TernaryTest extends TestCase
{
    public function testTernary()
    {
        $test = new \Test\Ternary();
        $this->assertSame(101, $test->testTernary1());
        $this->assertSame('foo', $test->testTernary2(true));
        $this->assertSame('bar', $test->testTernary2(false));

        $this->assertSame(3, $test->testTernaryAfterLetVariable());
        $this->assertSame(['', 'c', ''], $test->testTernaryWithPromotedTemporaryVariable());

        $this->assertTrue($test->testShortTernary(true));
        $this->assertFalse($test->testShortTernary([]));
        $this->assertSame([1, 2, 3], $test->testShortTernary([1, 2, 3]));
        $this->assertFalse($test->testShortTernary(false));
        $this->assertFalse($test->testShortTernary(0));

        $this->assertSame(1, $test->testShortTernaryComplex(false, 1));
        $this->assertSame('test string', $test->testShortTernaryComplex(false, 'test string'));
        $this->assertSame([], $test->testShortTernaryComplex(false, []));
    }

    public function testComplex()
    {
        $test = new \Test\Ternary();
        $this->assertSame(101, $test->testTernaryComplex1([], ''));
        $this->assertSame(106, $test->testTernaryComplex2([], ''));
        $this->assertSame('boolean', $test->testTernaryComplex3(''));
    }
}
