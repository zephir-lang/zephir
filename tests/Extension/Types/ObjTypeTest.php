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

namespace Extension\Types;

use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Types\Obj;

final class ObjTypeTest extends TestCase
{
    public function testIntFalse(): void
    {
        $class = new Obj();

        $this->assertNull($class->nullableObjectReturnNull());
        $this->assertInstanceOf(stdClass::class, $class->nullableObjectReturnObj());
    }
}
