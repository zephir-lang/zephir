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

class ResourceTest extends TestCase
{
    /** @var \Test\ResourceTest */
    private $test;

    public function setUp()
    {
        $this->test = new \Test\ResourceTest();
    }

    public function testLetStatementAssign()
    {
        $this->assertInternalType('resource', $this->test->testLetStatementSTDIN());
        $this->assertInternalType('resource', $this->test->testLetStatementSTDOUT());
        $this->assertInternalType('resource', $this->test->testLetStatementSTDERR());
    }

    public function testTypeOffResource()
    {
        $this->assertSame('resource', $this->test->testTypeOffResource());
    }

    public function testIsResource()
    {
        $this->assertTrue($this->test->testIsResource());
    }
}
