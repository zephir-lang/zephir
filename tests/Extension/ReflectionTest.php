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
use ReflectionClass;
use ReflectionFunction;
use Stub\Reflection as StubReflection;

final class ReflectionTest extends TestCase
{
    public function testMethods(): void
    {
        $class = new StubReflection();

        $this->assertInstanceOf(ReflectionClass::class, $class->getReflectionClass());
        $this->assertInstanceOf(ReflectionFunction::class, $class->getReflectionFunction());
    }
}
