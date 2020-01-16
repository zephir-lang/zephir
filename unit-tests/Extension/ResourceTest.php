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
    public function testLetStatementAssign()
    {
        $t = new \Test\ResourceTest();
        $this->assertInternalType('resource', $t->testLetStatementSTDIN());
        $this->assertInternalType('resource', $t->testLetStatementSTDOUT());
        $this->assertInternalType('resource', $t->testLetStatementSTDERR());
    }

    public function testTypeOffResource()
    {
        $t = new \Test\ResourceTest();
        $this->assertSame('resource', $t->testTypeOffResource());
    }

    public function testIsResource()
    {
        $t = new \Test\ResourceTest();
        $this->assertTrue($t->testIsResource());
    }
}
