<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue1704;

final class Issue1704Test extends TestCase
{
    private Issue1704 $test;

    protected function setUp(): void
    {
        $this->test = new Issue1704();
    }

    public function testBreakAfterReturnSatisfiesReturnType(): void
    {
        $this->assertSame('abc', $this->test->runmode(123));
    }

    public function testBreakAfterReturnThrowsOnDefault(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Invalid application runmode.');

        $this->test->runmode(456);
    }

    public function testPaddedClausesAllReturn(): void
    {
        $this->assertSame('one', $this->test->padded(1));
        $this->assertSame('two', $this->test->padded(2));
        $this->assertSame('many', $this->test->padded(99));
    }

    public function testDeadBreakInLoop(): void
    {
        $this->assertSame(0, $this->test->breakAfterReturnInLoop(1));
        $this->assertSame(-1, $this->test->breakAfterReturnInLoop(0));
    }

    public function testDeadContinueInLoop(): void
    {
        $this->assertSame(0, $this->test->continueAfterReturnInLoop(1));
        $this->assertSame(-1, $this->test->continueAfterReturnInLoop(0));
    }
}
