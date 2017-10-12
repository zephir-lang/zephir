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

class EmptyTest extends \PHPUnit_Framework_TestCase
{
    public function testEmpty()
    {
        $t = new \Test\EmptyTest();

        $this->assertTrue($t->testDynamicVarArrayEmpty());
        $this->assertFalse($t->testDynamicVarArrayNotEmpty());
        $this->assertTrue($t->testEmptyString());
        $this->assertFalse($t->testNotEmptyString());
        $this->assertTrue($t->testString(""));
        $this->assertFalse($t->testString('this is a string'));
        $this->assertTrue($t->testString('0'));
    }
}
