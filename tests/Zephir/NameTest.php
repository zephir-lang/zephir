<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Name;

final class NameTest extends TestCase
{
    public function testCamelize(): void
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(Name::camelize($name), 'FooBarFooBar');
    }
}
