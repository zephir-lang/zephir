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

namespace Extension\Types;

use PHPUnit\Framework\TestCase;
use Stub\Types\MayBe;

final class UnionTypesTest extends TestCase
{
    public function testIntFalse(): void
    {
        $class = new MayBe();

        $this->assertSame(1, $class->gc(123));
        $this->assertFalse($class->gcFalse());
    }
}
