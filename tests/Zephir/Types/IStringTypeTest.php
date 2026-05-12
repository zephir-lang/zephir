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
use Zephir\Types\IstringType;
use Zephir\Types\Types;

final class IStringTypeTest extends TestCase
{
    public function testShouldGetTypeName(): void
    {
        $stringType = new IstringType();

        $this->assertSame(Types::T_ISTRING, $stringType->getTypeName());
    }

    public function testShouldHaveMethodMap(): void
    {
        $stringType = new IstringType();

        $this->assertIsArray($stringType->methodMap);
        $this->assertNotEmpty($stringType->methodMap);
    }
}
