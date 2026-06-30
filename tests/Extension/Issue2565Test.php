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
use ReflectionFunction;
use Stub\Issue2565;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2565
 */
final class Issue2565Test extends TestCase
{
    public function testNarrowsTypeInsideInstanceOfGuard(): void
    {
        $object = new Issue2565();

        // A single named type reflects as ReflectionNamedType, so the
        // guarded getName() call (valid only on the subtype) must run.
        $named = (new ReflectionFunction(fn (int $x) => $x))->getParameters()[0]->getType();
        $this->assertSame('int', $object->resolveType($named));
    }

    public function testReturnsNullForNonNamedType(): void
    {
        $object = new Issue2565();

        // A union type reflects as ReflectionUnionType (a ReflectionType but
        // not a ReflectionNamedType), so the guard is skipped.
        $union = (new ReflectionFunction(fn (int|string $x) => $x))->getParameters()[0]->getType();
        $this->assertNull($object->resolveType($union));
    }
}
