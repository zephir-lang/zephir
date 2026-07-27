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
use ReflectionMethod;
use ReflectionProperty;
use ReflectionUnionType;
use Stub\Types\Unions;
use TypeError;

/**
 * Union types on parameters and class properties (issue #2613).
 */
final class UnionParamsPropsTest extends TestCase
{
    /**
     * Lowercased, sorted member names of a (union) type — order-independent.
     */
    private function memberNames(?\ReflectionType $type): array
    {
        $names = [];
        if ($type instanceof ReflectionUnionType) {
            foreach ($type->getTypes() as $member) {
                $names[] = strtolower($member->getName());
            }
        }
        sort($names);

        return $names;
    }

    public function testUnionParameterReflectsAsUnion(): void
    {
        $param = (new ReflectionMethod(Unions::class, 'kind'))->getParameters()[0];

        $this->assertInstanceOf(ReflectionUnionType::class, $param->getType());
        $this->assertSame(['float', 'int'], $this->memberNames($param->getType()));
    }

    public function testClassUnionParameterReflectsAsUnion(): void
    {
        $param = (new ReflectionMethod(Unions::class, 'pick'))->getParameters()[0];

        $this->assertInstanceOf(ReflectionUnionType::class, $param->getType());
        $this->assertSame(['string', 'stub\types\maybe'], $this->memberNames($param->getType()));
    }

    public function testUnionParameterPreservesRuntimeType(): void
    {
        $object = new Unions();

        // The value flows through with its natural runtime type (no coercion).
        $this->assertSame('integer', $object->kind(5));
        $this->assertSame('double', $object->kind(2.5));
    }

    public function testScalarUnionPropertyReflectsAsUnion(): void
    {
        $type = (new ReflectionProperty(Unions::class, 'num'))->getType();

        $this->assertInstanceOf(ReflectionUnionType::class, $type);
        $this->assertSame(['float', 'int'], $this->memberNames($type));
    }

    public function testNullableScalarUnionProperty(): void
    {
        $type = (new ReflectionProperty(Unions::class, 'note'))->getType();

        $this->assertSame(['int', 'null', 'string'], $this->memberNames($type));
    }

    public function testClassScalarUnionProperty(): void
    {
        $type = (new ReflectionProperty(Unions::class, 'tag'))->getType();

        $this->assertInstanceOf(ReflectionUnionType::class, $type);
        $this->assertSame(['string', 'stub\types\maybe'], $this->memberNames($type));
    }

    public function testScalarUnionPropertyAcceptsEitherMember(): void
    {
        $object      = new Unions();
        $object->num = 2;
        $this->assertSame(2, $object->num);
        $object->num = 2.5;
        $this->assertSame(2.5, $object->num);
    }

    public function testScalarUnionPropertyIsEnforcedOnWrite(): void
    {
        $object = new Unions();

        $this->expectException(TypeError::class);
        $object->num = [];
    }
}
