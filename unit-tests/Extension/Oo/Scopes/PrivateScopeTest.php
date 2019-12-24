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
use Test\Oo\Scopes\PrivateScopeTester;

class PrivateScopeTest extends TestCase
{
    /** @test */
    public function shouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }
}
