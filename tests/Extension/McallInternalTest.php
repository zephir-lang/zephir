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
use Stub\McallInternal;

final class McallInternalTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1956
     */
    public function testShouldReturnInstance(): void
    {
        $test = new McallInternal();
        $this->assertInstanceOf(McallInternal::class, $test->issue1956());
    }

    /**
     * Additional coverage for migrated blackbox test (t0007-internall-mcall.sh)
     * Ensures the method returns the exact same instance (internal call returning $this)
     * and that the class name matches expectation for completeness.
     */
    public function testIssue1956ReturnsSameInstanceAndClassName(): void
    {
        $instance = new McallInternal();
        $returned = $instance->issue1956();

        // Class assertion (mirrors original blackbox intent of get_class output)
        $this->assertSame(McallInternal::class, get_class($returned));

        // Identity assertion (should be the exact same object instance)
        $this->assertSame($instance, $returned, 'issue1956() must return the same instance (i.e., $this)');

        // Chaining safety: calling again on the returned instance should still yield itself
        $this->assertSame($instance, $returned->issue1956());
    }

    /**
     * An internal method with parameters must receive its own arguments.
     *
     * `g()` takes no arguments of its own and calls `c(long, long)` in a loop.
     * The internal method used to parse the caller's argument frame, so both of
     * these crashed the process outright.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2021
     */
    public function testInternalMethodWithParametersReceivesItsOwnArguments(): void
    {
        // sum of (i + i) for i in 0..1000 == 2 * 500500
        $this->assertSame(1001000, (new McallInternal())->g());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2021
     */
    public function testInternalMethodReturningDoubleReceivesItsOwnArguments(): void
    {
        // sum of i / (i + 1) for i in 0..10000000 == 10000001 - H(10000001)
        $this->assertEqualsWithDelta(9999984.3046, (new McallInternal())->callFibonacci(), 0.001);
    }
}
