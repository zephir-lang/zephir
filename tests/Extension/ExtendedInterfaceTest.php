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
use Stub\ExtendedInterface;

final class ExtendedInterfaceTest extends TestCase
{
    public function testShouldExtendsInterfaces(): void
    {
        $reflection = new \ReflectionClass(ExtendedInterface::class);

        $this->assertTrue($reflection->isInterface());
        $this->assertContains('IteratorAggregate', $reflection->getInterfaceNames());
        $this->assertContains('Countable', $reflection->getInterfaceNames());
    }
}
