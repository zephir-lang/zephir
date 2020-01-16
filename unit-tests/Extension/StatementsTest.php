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
use Test\Statements;

class StatementsTest extends TestCase
{
    public function testElseIf()
    {
        $t = new Statements();
        $this->assertSame('more', $t->testElseIf(10));
        $this->assertSame('equal', $t->testElseIf(0));
        $this->assertSame('-1', $t->testElseIf(-1));
        $this->assertSame('less', $t->testElseIf(-10));
    }

    public function testElseIf1()
    {
        $t = new Statements();
        $this->assertSame('equal', $t->testElseIf1(10));
        $this->assertSame('less', $t->testElseIf1(5));
        $this->assertSame('else', $t->testElseIf1(11));
    }

    public function testElseIf2()
    {
        $t = new Statements();
        $this->assertSame('equal', $t->testElseIf2(10, 10));
        $this->assertSame('less', $t->testElseIf2(5, 10));
        $this->assertSame('else', $t->testElseIf2(11, 10));
    }

    public function test544Issue()
    {
        $t = new Statements();

        $this->assertSame('====>...............................................................................................', $t->test544Issue(5));
        $this->assertSame('====================================================================================================', $t->test544Issue(100));

        $this->assertSame('====>...............................................................................................', $t->test544IssueWithVariable(5));
        $this->assertSame('====================================================================================================', $t->test544IssueWithVariable(100));
    }
}
