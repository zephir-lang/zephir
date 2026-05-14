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
use Zephir\Types\ArrayType;
use Zephir\Types\Types;

final class ArrayTypeTest extends TestCase
{
    public function testShouldGetTypeName(): void
    {
        $arrayType = new ArrayType();

        $this->assertSame(Types::T_ARRAY, $arrayType->getTypeName());
    }

    public function testShouldHaveMethodMap(): void
    {
        $arrayType = new ArrayType();

        $this->assertIsArray($arrayType->methodMap);
        $this->assertNotEmpty($arrayType->methodMap);
    }
}
