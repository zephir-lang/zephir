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
use Zephir\Types\StringType;
use Zephir\Types\Types;

final class StringTypeTest extends TestCase
{
    public function testShouldGetTypeName(): void
    {
        $stringType = new StringType();

        $this->assertSame(Types::T_STRING, $stringType->getTypeName());
    }

    public function testShouldHaveMethodMap(): void
    {
        $stringType = new StringType();

        $this->assertIsArray($stringType->methodMap);
        $this->assertNotEmpty($stringType->methodMap);
    }
}
