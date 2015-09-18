<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Extension\Optimizers;

use Test\Optimizers\StrReplace;

class StrReplaceTest extends \PHPUnit_Framework_TestCase
{
    public function testIssue1055()
    {
        $this->assertEquals(StrReplace::issue1055(), "bla bla aaa aaaa askks");
    }

    public static function issue1087()
    {
        $this->assertEquals(StrReplace::issue1087(), "test_test");
    }

    public static function issue732()
    {
        $this->assertEquals(StrReplace::issue732A(), "F");
        $this->assertEquals(StrReplace::issue732B(), "The slow black bear jumped over the lazy dog.");
    }
}
