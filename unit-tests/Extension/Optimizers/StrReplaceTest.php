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

namespace Extension\Optimizers;

use Test\Optimizers\StrReplace;

class StrReplaceTest extends \PHPUnit_Framework_TestCase
{
    public function testIssue1055()
    {
        $this->assertSame(StrReplace::issue1055(), "bla bla aaa aaaa askks");
    }

    public static function issue1087()
    {
        $this->assertSame(StrReplace::issue1087(), "test_test");
    }

    public static function issue732()
    {
        $this->assertSame(StrReplace::issue732A(), "F");
        $this->assertSame(StrReplace::issue732B(), "The slow black bear jumped over the lazy dog.");
    }
}
