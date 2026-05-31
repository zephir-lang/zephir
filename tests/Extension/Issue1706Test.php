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
use Stub\Issue1706;

final class Issue1706Test extends TestCase
{
    private Issue1706 $test;

    protected function setUp(): void
    {
        $this->test = new Issue1706();
    }

    public function testSwitch(): void
    {
        $this->assertSame(1, $this->test->testSwitch());
    }

    public function testSwitchDefaultLast(): void
    {
        $this->assertSame(10, $this->test->switchDefaultLast(1));
        $this->assertSame(20, $this->test->switchDefaultLast(2));
        $this->assertSame(0, $this->test->switchDefaultLast(99));
    }

    public function testSwitchWithThrow(): void
    {
        $this->assertSame('one', $this->test->switchWithThrow(1));

        $this->expectException(\Exception::class);
        $this->test->switchWithThrow(2);
    }
}
