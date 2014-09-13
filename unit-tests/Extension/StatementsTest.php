<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\Statements;

class StatementsTest extends \PHPUnit_Framework_TestCase
{
    public function testElseIf()
    {
        $t = new Statements;
        $this->assertEquals('more', $t->testElseIf(10));
        $this->assertEquals('equal', $t->testElseIf(0));
        $this->assertEquals('-1', $t->testElseIf(-1));
        $this->assertEquals('less', $t->testElseIf(-10));
    }
}
