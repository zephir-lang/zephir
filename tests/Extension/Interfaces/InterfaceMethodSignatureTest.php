<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
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

        $this->assertNull($class->get());
        $class->set(1);
        $this->assertSame(1, $class->get());

        $this->assertSame(1, (new ImplementInterface())->get($class));

        $error = '';
        try {
            (new ImplementInterface())->getVoid(true);
        } catch (\TypeError $exception) {
            $error = $exception->getMessage();
        }

        $this->assertMatchesRegularExpression('/^must be of type InterfaceInt, bool given/', $error);
    }
}
