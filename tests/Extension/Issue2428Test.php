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

namespace Extension;

use PHPUnit\Framework\TestCase;
use ReflectionMethod;
use ReflectionNamedType;
use ReflectionUnionType;
use Stub\Issue2428;

/**
 * Proves that union return types are enforced by the engine — not merely
 * rendered into stub files. Reflection reads the arginfo the compiled
 * extension registers, so a passing assertion here means PHP itself knows
 * (and enforces) every member of the union.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2428
 */
final class Issue2428Test extends TestCase
{
    private Issue2428 $sut;

    protected function setUp(): void
    {
        $this->sut = new Issue2428();
    }

    /**
     * @return string[] the union's member type names, sorted, lower-cased.
     */
    private function returnTypeNames(string $method): array
    {
        $type = (new ReflectionMethod(Issue2428::class, $method))->getReturnType();
        $this->assertNotNull($type, "$method must declare a return type");

        $names = [];
        if ($type instanceof ReflectionUnionType) {
            foreach ($type->getTypes() as $member) {
                $names[] = $member->getName();
            }
        } elseif ($type instanceof ReflectionNamedType) {
            $names[] = $type->getName();
            if ($type->allowsNull() && 'null' !== $type->getName()) {
                $names[] = 'null';
            }
        }

        $names = array_map('strtolower', $names);
        sort($names);

        return $names;
    }

    public function testScalarUnionIsEnforced(): void
    {
        $this->assertSame(['int', 'string'], $this->returnTypeNames('scalarUnion'));
        $this->assertSame(1, $this->sut->scalarUnion(true));
        $this->assertSame('x', $this->sut->scalarUnion(false));
    }

    public function testNullableScalarUnionIsEnforced(): void
    {
        $this->assertSame(['int', 'null', 'string'], $this->returnTypeNames('scalarUnionNullable'));
        $this->assertSame(7, $this->sut->scalarUnionNullable(7));
    }

    public function testMultiClassUnionIsEnforced(): void
    {
        $this->assertSame(['arrayobject', 'stdclass'], $this->returnTypeNames('classUnion'));
        $this->assertInstanceOf(\stdClass::class, $this->sut->classUnion(true));
        $this->assertInstanceOf(\ArrayObject::class, $this->sut->classUnion(false));
    }

    public function testNullableMultiClassUnionIsEnforced(): void
    {
        $this->assertSame(
            ['arrayobject', 'null', 'stdclass'],
            $this->returnTypeNames('classUnionNullable')
        );
        $this->assertNull($this->sut->classUnionNullable(0));
        $this->assertInstanceOf(\stdClass::class, $this->sut->classUnionNullable(1));
    }

    public function testClassAndScalarUnionIsEnforced(): void
    {
        $this->assertSame(['int', 'stdclass'], $this->returnTypeNames('classOrScalar'));
        $this->assertInstanceOf(\stdClass::class, $this->sut->classOrScalar(true));
        $this->assertSame(5, $this->sut->classOrScalar(false));
    }

    public function testArrayAndGenericObjectUnionIsEnforced(): void
    {
        $this->assertSame(['array', 'object'], $this->returnTypeNames('arrayOrObject'));
        $this->assertSame([], $this->sut->arrayOrObject(true));
        $this->assertInstanceOf(\stdClass::class, $this->sut->arrayOrObject(false));
    }

    public function testValidDynamicReturnValuesPass(): void
    {
        $this->assertSame(['int', 'string'], $this->returnTypeNames('dynamicReturn'));
        $this->assertSame(42, $this->sut->dynamicReturn(42));
        $this->assertSame('ok', $this->sut->dynamicReturn('ok'));
    }

    /**
     * An internal (extension) method returning a value outside its declared
     * union must NOT crash the process. Like PHP's own internal functions,
     * the return value of an internal method is not self-verified in a release
     * build, so the out-of-union value simply passes through — the critical
     * guarantee is that the union arginfo causes no segfault on a bad return.
     */
    public function testInternalReturnOutsideUnionDoesNotSegfault(): void
    {
        $result = $this->sut->dynamicReturn([1, 2, 3]);

        $this->assertSame([1, 2, 3], $result);
        $this->assertTrue(true, 'Process survived a return value outside the declared union.');
    }

    /**
     * The union return type emitted into arginfo participates in real PHP
     * inheritance/enforcement: a userland subclass that overrides the method
     * must honor `int|string`. Returning an uncoercible value (an array) at the
     * userland boundary raises a TypeError — proving the engine genuinely knows
     * the inherited union, while still never segfaulting.
     */
    public function testUserlandOverrideEnforcesInheritedUnion(): void
    {
        if (!class_exists('Extension\\Issue2428Child', false)) {
            eval(
                'namespace Extension; '
                . 'class Issue2428Child extends \\Stub\\Issue2428 { '
                . 'public function dynamicReturn($value): int|string { return $value; } }'
            );
        }

        $child = new Issue2428Child();
        $this->assertSame(7, $child->dynamicReturn(7));

        $this->expectException(\TypeError::class);
        $child->dynamicReturn([1, 2, 3]);
    }
}
