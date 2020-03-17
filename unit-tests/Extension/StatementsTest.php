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
    /** @var Statements */
    private $test;

    public function setUp()
    {
        $this->test = new Statements();
    }

    public function testElseIf()
    {
        $this->assertSame('more', $this->test->testElseIf(10));
        $this->assertSame('equal', $this->test->testElseIf(0));
        $this->assertSame('-1', $this->test->testElseIf(-1));
        $this->assertSame('less', $this->test->testElseIf(-10));
    }

    public function testElseIf1()
    {
        $this->assertSame('equal', $this->test->testElseIf1(10));
        $this->assertSame('less', $this->test->testElseIf1(5));
        $this->assertSame('else', $this->test->testElseIf1(11));
    }

    public function testElseIf2()
    {
        $this->assertSame('equal', $this->test->testElseIf2(10, 10));
        $this->assertSame('less', $this->test->testElseIf2(5, 10));
        $this->assertSame('else', $this->test->testElseIf2(11, 10));
    }

    public function test544Issue()
    {
        $this->assertSame('====>...............................................................................................', $this->test->test544Issue(5));
        $this->assertSame('====================================================================================================', $this->test->test544Issue(100));

        $this->assertSame('====>...............................................................................................', $this->test->test544IssueWithVariable(5));
        $this->assertSame('====================================================================================================', $this->test->test544IssueWithVariable(100));
    }
}
