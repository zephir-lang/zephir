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

use Test\Requires;

class RequiresTest extends \PHPUnit_Framework_TestCase
{
    public function testRequireExternal1()
    {
        $r = new Requires();
        $this->assertSame($r->requireExternal1(__DIR__ . '/php/require-me-1.php'), array(1, 2, 3));
        $this->assertTrue($r->requireExternal1(__DIR__ . '/php/require-me-2.php') && defined('REQUIRE_ME'));
    }

    public function testRequireExternal3()
    {
        $r = new Requires();
        $output = $r->requireExternal3(__DIR__ . '/php/require-me-3.php');
        $this->assertSame("test", $output);
    }
}
