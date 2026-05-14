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

namespace Extension\Interfaces;

use PHPUnit\Framework\TestCase;
use Stub\Interfaces\ImplementInt;
use Stub\Interfaces\ImplementInterface;

final class InterfaceMethodSignatureTest extends TestCase
{
    public function testImplementInterfaceInMethodSignature(): void
    {
        $class = new ImplementInt();

        // ImplementInt::val now defaults to 0 instead of null so that
        // `get(): int` doesn't violate its own return type (#1991).
        $this->assertSame(0, $class->get());
        $class->set(1);
        $this->assertSame(1, $class->get());
        $this->assertSame(1, (new ImplementInterface())->get($class));
    }

    public function testImplementInterfaceInMethodSignatureInt(): void
    {
        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageMatches('/must be of type int, (true|bool) given/');

        (new ImplementInt())->set(true);
    }

    public function testImplementInterfaceInMethodSignatureInterface(): void
    {
        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageMatches(
            '/(must be of type) Stub\\\\Interfaces\\\\InterfaceInt, (true|bool) given/'
        );

        (new ImplementInterface())->getVoid(true);
    }
}
