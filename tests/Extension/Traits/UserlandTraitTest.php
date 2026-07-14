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

namespace Extension\Traits;

use PHPUnit\Framework\TestCase;
use ReflectionClass;

final class UserlandTraitTest extends TestCase
{
    public function testTraitIsExposedToUserland(): void
    {
        $this->assertTrue(trait_exists('Stub\Traits\Nameable'));
        $this->assertTrue((new ReflectionClass('Stub\Traits\Nameable'))->isTrait());
    }

    public function testTraitCannotBeInstantiated(): void
    {
        $this->expectException(\Error::class);

        new \Stub\Traits\Nameable();
    }

    public function testPhpClassCanUseZephirTrait(): void
    {
        $object = new class {
            use \Stub\Traits\Nameable;
        };

        $this->assertSame('unnamed', $object->getName());

        $object->setName('zephir');

        $this->assertSame('zephir', $object->getName());
        $this->assertContains('Stub\Traits\Nameable', class_uses($object));
    }

    /**
     * The array default declared on the trait must be carried into a PHP
     * userland class by native trait binding (issue #2607) — this is the
     * exact case the old compile-time guard forbade.
     */
    public function testArrayPropertyDefaultIsCarriedIntoUserlandClass(): void
    {
        $object = new class {
            use \Stub\Traits\Nameable;
        };

        $this->assertSame([1, 2, 3], $object->getTags());
        $this->assertSame(['a' => 1, 'b' => [2, 3]], $object->getMeta());
    }

    /**
     * Two independent instances must not share the array default (copy-on-write,
     * identical to a native PHP array property default).
     */
    public function testArrayPropertyDefaultIsNotSharedBetweenInstances(): void
    {
        $klass = new class {
            use \Stub\Traits\Nameable;
        };

        $a = new $klass();
        $b = new $klass();

        $a->addTag(4);

        $this->assertSame([1, 2, 3, 4], $a->getTags());
        $this->assertSame([1, 2, 3], $b->getTags());
    }
}
