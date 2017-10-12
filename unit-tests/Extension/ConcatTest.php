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

use Test\Concat;

class ConcatTest extends \PHPUnit_Framework_TestCase
{
    public function testStaticPropertyConcatBySelfContext()
    {
        $t = new Concat();
        $this->assertSame(null, $t->getTestProperty());
        $t->testConcatBySelfProperty("test");
        $this->assertSame("test", $t->getTestProperty());
        $t->testConcatBySelfProperty(" string");
        $this->assertSame("test string", $t->getTestProperty());
    }
}
