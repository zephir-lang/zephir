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

use Test\McallDynamic;

class MCallTestDynamic extends \PHPUnit_Framework_TestCase
{
    public function testCallDynamic()
    {
        $a = new McallDynamic;
        $this->assertSame($a->method1(), 1);
        $this->assertSame($a->testMagicCall1(), 2);
    }
}
