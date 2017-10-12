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

namespace Extension\Oo;

use Test\Oo\DeprecatedMethods;

class DeprecatedMethodsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @expectedException \PHPUnit_Framework_Error_Deprecated
     */
    public function testPublicMethodThrowsDeprecatedWarning()
    {
        $test = new DeprecatedMethods;
        $test->deprecatedMethod();
    }

    /**
     * @expectedException \PHPUnit_Framework_Error_Deprecated
     */
    public function testPrivateMethodThrowsDeprecatedWarning()
    {
        $test = new DeprecatedMethods;
        $test->normalMethod();
    }
}
