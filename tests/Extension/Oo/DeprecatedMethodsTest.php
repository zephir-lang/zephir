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
use Symfony\Bridge\PhpUnit\ExpectDeprecationTrait;

class DeprecatedMethodsTest extends TestCase
{
    use ExpectDeprecationTrait;

    /**
     * @group legacy
     */
    public function testPublicMethodThrowsDeprecatedWarning(): void
    {
        $this->expectDeprecation('Unsilenced deprecation: Method Stub\Oo\DeprecatedMethods::publicDeprecated() is deprecated');

        $test = new DeprecatedMethods();
        $test->publicDeprecated();
    }

    /**
     * @group legacy
     */
    public function testPrivateMethodThrowsDeprecatedWarning(): void
    {
        $this->expectDeprecation('Unsilenced deprecation: Method Stub\Oo\DeprecatedMethods::privateDeprecated() is deprecated');

        $test = new DeprecatedMethods();
        $test->normalMethod();
    }
}
