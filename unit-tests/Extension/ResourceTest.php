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

class ResourceTest extends \PHPUnit_Framework_TestCase
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
