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
}
