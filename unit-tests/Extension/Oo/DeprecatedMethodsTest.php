<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use Test\Oo\DeprecatedMethods;
use Zephir\Support\TestCase;

class DeprecatedMethodsTest extends TestCase
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
