<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use Test\Oo\Scopes\PrivateScopeTester;
use Zephir\Support\TestCase;

class PrivateScopeTest extends TestCase
{
    /** @test */
    public function shouldCallPrivateMethod()
    {
        if (PHP_VERSION_ID < 50600) {
            $this->markTestSkipped(
                "Calling parent's private methods from the child's public ones doesn't provided for PHP < 5.6"
            );
        }

        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }
}
