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

use Error;
use PHPUnit\Framework\TestCase;
use ReflectionProperty;
use Stub\Properties\PublicProperties;
use Stub\Properties\ReadonlyProperties;

/**
 * Readonly class properties (issue #2614): the property carries
 * ZEND_ACC_READONLY, so Reflection reports it and the engine enforces
 * write-once after initialization. Enforced on PHP 8.1+.
 */
final class ReadonlyPropertiesTest extends TestCase
{
    protected function setUp(): void
    {
        if (PHP_VERSION_ID < 80100) {
            $this->markTestSkipped('readonly properties require PHP 8.1+');
        }
    }

    /**
     * @dataProvider readonlyProvider
     */
    public function testPropertiesAreReadonlyAndTyped(string $property, string $type): void
    {
        $rp = new ReflectionProperty(ReadonlyProperties::class, $property);

        $this->assertTrue($rp->isReadOnly(), "$property must be readonly");
        $this->assertTrue($rp->hasType(), "$property must have a type");
        $this->assertSame($type, (string) $rp->getType());
    }

    public static function readonlyProvider(): array
    {
        return [
            ['id', 'int'],
            ['label', 'string'],
            ['tags', 'array'],
            ['flag', 'bool'],
            ['friend', PublicProperties::class],
        ];
    }

    public function testConstructorInitializesValues(): void
    {
        $test = new ReadonlyProperties();

        $this->assertSame(42, $test->id);
        $this->assertSame('hello', $test->label);
        $this->assertSame(['a', 'b'], $test->tags);
        $this->assertTrue($test->getFlag());
        $this->assertInstanceOf(PublicProperties::class, $test->friend);
    }

    public function testReassignmentFromUserlandThrows(): void
    {
        $test = new ReadonlyProperties();

        $this->expectException(Error::class);
        $this->expectExceptionMessage('Cannot modify readonly property');
        /** @phpstan-ignore-next-line intentional write to a readonly property */
        $test->id = 100;
    }
}
