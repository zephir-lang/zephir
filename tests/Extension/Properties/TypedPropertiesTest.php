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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use ReflectionProperty;
use Stub\Properties\PublicProperties;
use Stub\Properties\TypedProperties;
use TypeError;

/**
 * Typed class properties (issue #2608): the declared type is reported by
 * Reflection and enforced by the engine.
 */
final class TypedPropertiesTest extends TestCase
{
    /**
     * @dataProvider typeProvider
     */
    public function testPropertyTypesAreReported(string $property, string $type, bool $nullable): void
    {
        $rp = new ReflectionProperty(TypedProperties::class, $property);

        $this->assertTrue($rp->hasType(), "$property must have a type");
        $this->assertSame($type, (string) $rp->getType());
        $this->assertSame($nullable, $rp->getType()->allowsNull());
    }

    public static function typeProvider(): array
    {
        return [
            ['orgs', 'array', false],
            ['valid', 'bool', false],
            ['counter', 'int', false],
            ['ratio', 'float', false],
            ['label', 'string', false],
            ['val', '?string', true],
            ['maybeNum', '?int', true],
            ['friend', PublicProperties::class, false],
            ['maybeFriend', '?' . PublicProperties::class, true],
        ];
    }

    public function testScalarAndArrayDefaults(): void
    {
        $test = new TypedProperties();

        $this->assertSame([], $test->orgs);
        $this->assertSame(7, $test->counter);
        $this->assertSame(1.5, $test->ratio);
        $this->assertSame('hi', $test->label);
        $this->assertSame('', $test->getVal());
        $this->assertNull($test->maybeNum);
        $this->assertNull($test->maybeFriend);
    }

    public function testUninitializedClassPropertyIsNotInitialized(): void
    {
        $test = new TypedProperties();
        $rp   = new ReflectionProperty(TypedProperties::class, 'friend');

        $this->assertFalse($rp->isInitialized($test));

        $this->expectException(\Error::class);
        $this->expectExceptionMessage('must not be accessed before initialization');
        /** @phpstan-ignore-next-line intentional read of uninitialized property */
        $test->friend;
    }

    public function testTypeIsEnforcedOnAssignment(): void
    {
        $test = new TypedProperties();

        $this->expectException(TypeError::class);
        /** @phpstan-ignore-next-line intentional wrong-type assignment */
        $test->counter = [];
    }

    public function testClassTypeIsEnforcedOnAssignment(): void
    {
        $test   = new TypedProperties();
        $friend = new PublicProperties();

        $test->friend = $friend;
        $this->assertSame($friend, $test->friend);

        $this->expectException(TypeError::class);
        /** @phpstan-ignore-next-line intentional wrong-type assignment */
        $test->maybeFriend = new \stdClass();
    }
}
