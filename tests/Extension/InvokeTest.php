<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Invokes\InvokeProtected;

class InvokeTest extends TestCase
{
    public function testIssue1522(): void
    {
        $x1 = \Stub\Invoke::test();
        $x2 = \Stub\Invoke::test();
        $this->assertSame('string', $x1);
        $this->assertSame('string', $x2);
    }

    public function testProtectedMethod(): void
    {
        $fromProtected = new InvokeProtected('random');

        $this->assertInstanceOf(InvokeProtected::class, $fromProtected());
        $this->assertSame('random2', (string)$fromProtected());
        $this->assertSame('random2', $fromProtected()->__toString());
    }
}
