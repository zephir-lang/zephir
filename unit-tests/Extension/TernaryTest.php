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
        $t = new \Test\Ternary();
        $this->assertSame(101, $t->testTernary1());
        $this->assertSame('foo', $t->testTernary2(true));
        $this->assertSame('bar', $t->testTernary2(false));

        $this->assertSame(3, $t->testTernaryAfterLetVariable());
        $this->assertSame(['', 'c', ''], $t->testTernaryWithPromotedTemporaryVariable());

        $this->assertTrue($t->testShortTernary(true));
        $this->assertFalse($t->testShortTernary([]));
        $this->assertSame([1, 2, 3], $t->testShortTernary([1, 2, 3]));
        $this->assertFalse($t->testShortTernary(false));
        $this->assertFalse($t->testShortTernary(0));

        $this->assertSame(1, $t->testShortTernaryComplex(false, 1));
        $this->assertSame('test string', $t->testShortTernaryComplex(false, 'test string'));
        $this->assertSame([], $t->testShortTernaryComplex(false, []));
    }

    public function testComplex()
    {
        $t = new \Test\Ternary();
        $this->assertSame(101, $t->testTernaryComplex1([], ''));
        $this->assertSame(106, $t->testTernaryComplex2([], ''));
        $this->assertSame('boolean', $t->testTernaryComplex3(''));
    }
}
