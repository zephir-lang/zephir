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
use Zephir\Types\IntType;
use Zephir\Types\Types;

final class IntTypeTest extends TestCase
{
    public function testShouldGetTypeName(): void
    {
        $intType = new IntType();

        $this->assertSame(Types::T_INT, $intType->getTypeName());
    }

    public function testShouldHaveMethodMap(): void
    {
        $intType = new IntType();

        $this->assertIsArray($intType->methodMap);
    }
}
