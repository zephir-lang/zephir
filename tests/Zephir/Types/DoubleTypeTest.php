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

namespace Zephir\Test\Types;

use PHPUnit\Framework\TestCase;
use Zephir\Types\DoubleType;
use Zephir\Types\Types;

final class DoubleTypeTest extends TestCase
{
    public function testShouldGetTypeName(): void
    {
        $doubleType = new DoubleType();

        $this->assertSame(Types::T_DOUBLE, $doubleType->getTypeName());
    }

    public function testShouldHaveMethodMap(): void
    {
        $doubleType = new DoubleType();

        $this->assertIsArray($doubleType->methodMap);
    }
}
