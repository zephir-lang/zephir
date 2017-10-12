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

namespace Zephir\Test;

use Zephir\Utils;

class UtilsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test escapeClassName method.
     */
    public function testEscapeClassName()
    {
        $classname = '\Bar\Foo';
        $this->assertSame(
            Utils::escapeClassName($classname),
            '\\\\Bar\\\\Foo'
        );
    }

    /**
     * Test camelize method.
     */
    public function testCamelize()
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(
            Utils::camelize($name),
            'FooBarFooBar'
        );
    }
}
