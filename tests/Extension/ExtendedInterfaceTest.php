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

    /**
     * Unlike the Zend interfaces above, these two live in PHP extension
     * headers, so the generated interface C only compiles once those headers
     * are included alongside the `zend_class_implements()` calls.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2427
     */
    public function testShouldExtendsInterfacesFromPhpExtensions(): void
    {
        $reflection = new \ReflectionClass(ExtendedInterface::class);

        $this->assertContains('JsonSerializable', $reflection->getInterfaceNames());
        $this->assertContains('SplObserver', $reflection->getInterfaceNames());
    }
}
