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

use \Test\SPropertyAccess;

class SPropertyAccessTest extends \PHPUnit_Framework_TestCase
{
    public function testStaticPropertyAccess()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgument(","), ",");
        $this->assertSame($spa->testArgument(), ".");
    }

    public function testStaticPropertyAccessWithUnderscore()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgumentWithUnderscore(","), ",");
        $this->assertSame($spa->testArgumentWithUnderscore(), ".");
    }
}
