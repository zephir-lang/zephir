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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\StaticPropertyArray;

final class StaticPropertyArrayTest extends TestCase
{
    public function testAssertations(): void
    {
        $this->assertSame([], StaticPropertyArray::$someEmptyArray);
        $this->assertSame([1, 2, 3, 4], StaticPropertyArray::$someArray);
    }
}
