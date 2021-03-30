<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

final class DefaultsTest extends TestCase
{
    public function testDefaultNullOnString(): void
    {
        $test = new \Stub\Defaults();

        $this->assertSame('hello', $test->defaultVarNull('hello'));
        $this->assertSame(null, $test->defaultVarNull());

        $this->assertSame('hello', $test->defaultStringNull('hello'));
        $this->assertSame(null, $test->defaultStringNull());
    }
}
