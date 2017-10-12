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

class Issue1521Test extends \PHPUnit_Framework_TestCase
{
    public function testIssue1521()
    {
        $t      = new \Test\Issue1521();
        $actual = $t->test();
        $this->assertTrue(0 === $actual['zero']);
        $this->assertTrue("0818217122" === $actual['merchantNumber']);
    }
}
