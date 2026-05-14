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
}
