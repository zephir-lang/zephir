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

namespace Extension\Flow;

use PHPUnit\Framework\TestCase;
use Stub\Flow\SwitchFlow;

final class SwitchFlowTest extends TestCase
{
    public function testSwitch(): void
    {
        $test = new SwitchFlow();

        $this->assertSame(10, $test->testSwitch1());
        $this->assertTrue($test->testSwitch2());
        $this->assertFalse($test->testSwitch3());
        $this->assertSame(2, $test->testSwitch4());
        $this->assertSame(1, $test->testSwitch5());
        $this->assertSame(1, $test->testSwitch6());
        $this->assertSame(1, $test->testSwitch7());
        $this->assertSame(2, $test->testSwitch8());
        $this->assertSame(2, $test->testSwitch9());
        $this->assertSame(2, $test->testSwitch10());
        $this->assertSame(1, $test->testSwitch11());
        $this->assertSame(1, $test->testSwitch12(10, 6));
        $this->assertSame(0, $test->testSwitch12(6, 10));
        $this->assertSame(2, $test->testSwitch12(2, 3));
        $this->assertSame('aaa', $test->testSwitch14(1));
        $this->assertSame('bbb', $test->testSwitch14(99));
    }
}
