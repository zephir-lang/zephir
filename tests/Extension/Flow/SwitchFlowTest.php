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

    /**
     * A clause without `break` falls through into the next clause's body, and
     * on into `default`, exactly as in PHP.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public function testFallThrough(): void
    {
        $test = new SwitchFlow();

        $this->assertSame('abd', $test->testSwitch15(1));
        $this->assertSame('bd', $test->testSwitch15(2));
        $this->assertSame('d', $test->testSwitch15(9));
    }

    /**
     * A `default` clause in the middle falls through into the `case` after it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public function testDefaultInTheMiddleFallsThrough(): void
    {
        $test = new SwitchFlow();

        $this->assertSame('one', $test->testSwitch16(1));
        $this->assertSame('two', $test->testSwitch16(2));
        $this->assertSame('deftwo', $test->testSwitch16(9));
    }

    /**
     * `continue` inside a `switch` is equivalent to `break`: the loop body
     * after the `switch` still runs on that iteration.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public function testContinueInsideSwitchBehavesLikeBreak(): void
    {
        $this->assertSame('0123', (new SwitchFlow())->testSwitch17());
    }

    /**
     * A `break` inside a loop nested in a clause targets the loop.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public function testBreakInsideNestedLoop(): void
    {
        $this->assertSame('w3', (new SwitchFlow())->testSwitch18());
    }

    /**
     * An empty clause falls through into the next body, then into `default`.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public function testEmptyClauseFallsThrough(): void
    {
        $test = new SwitchFlow();

        $this->assertSame('xy', $test->testSwitch19(1));
        $this->assertSame('xy', $test->testSwitch19(2));
        $this->assertSame('y', $test->testSwitch19(9));
    }
}
