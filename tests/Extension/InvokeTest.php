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
use Stub\Invoke;
use Stub\Invokes\InvokeProtected;
use Stub\Invokes\InvokeProtectedComplex;

final class InvokeTest extends TestCase
{
    public function testIssue1522(): void
    {
        $x1 = Invoke::test();
        $x2 = Invoke::test();
        $this->assertSame('string', $x1);
        $this->assertSame('string', $x2);
    }

    public function testProtectedMethod(): void
    {
        $fromProtected = new InvokeProtected('random');

        $this->assertInstanceOf(InvokeProtected::class, $fromProtected());
        $this->assertSame('random2', (string) $fromProtected());
        $this->assertSame('random2', $fromProtected()->__toString());
    }

    public function testProtectedMethodComplex(): void
    {
        $expected = 'random1random2';
        $fromProtected = new InvokeProtectedComplex('random');

        $this->assertInstanceOf(InvokeProtectedComplex::class, $fromProtected());
        $this->assertSame($expected, (string) $fromProtected());
        $this->assertSame($expected, $fromProtected()->__toString());
    }
}
