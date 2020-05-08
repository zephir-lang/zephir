<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use PHPUnit\Framework\TestCase;
use Stub\Oo\DeprecatedMethods;

class DeprecatedMethodsTest extends TestCase
{
    /**
     * @group legacy
     * @expectedDeprecation Unsilenced deprecation: Function %s is deprecated
     */
    public function testPublicMethodThrowsDeprecatedWarning()
    {
        $test = new DeprecatedMethods();
        $test->publicDeprecated();
    }

    /**
     * @group legacy
     * @expectedDeprecation Unsilenced deprecation: Function %s is deprecated
     */
    public function testPrivateMethodThrowsDeprecatedWarning()
    {
        $test = new DeprecatedMethods();
        $test->normalMethod();
    }
}
