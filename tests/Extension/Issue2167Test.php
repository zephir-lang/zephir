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
use Stub\Issue2167;
use TypeError;

/**
 * Issue #2167: passing `[ClassName, "method"]` directly to a PHP
 * callable-validating function fails when the method is non-public,
 * because Zephir methods are ZEND_INTERNAL_FUNCTION and PHP's
 * `zend_get_executed_scope()` skips them. The workaround is to wrap
 * the callback in a Zephir closure that does the delegating call via
 * a fully-qualified class name.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2167
 * @see https://github.com/zephir-lang/zephir/issues/2321 (same root cause)
 */
final class Issue2167Test extends TestCase
{
    private const SAMPLE = [[1, 2, 3], [4, 5, 6]];
    private const EXPECTED_OUTPUT = '[ 1 2 3 ][ 4 5 6 ]';

    /**
     * Lock down the documented failure mode of the direct
     * `[ClassName, "protectedMethod"]` form so a future kernel-level
     * fix is visible as a regression here (the assertion will start
     * failing because the call succeeds, signalling that the
     * workaround documentation can be downgraded).
     */
    public function testDirectArrayCallableFailsForProtectedMethod(): void
    {
        $sut = new Issue2167(self::SAMPLE);

        $this->expectException(TypeError::class);
        $this->expectExceptionMessage('cannot access protected method Stub\\Issue2167::implodeRow');

        $sut->reduceWithArrayCallable();
    }

    /**
     * The supported workaround: a closure that delegates via the
     * fully-qualified static class name. The closure is bound to the
     * enclosing class, so PHP's visibility check sees the right scope.
     */
    public function testClosureWrapperWorksForProtectedMethod(): void
    {
        $sut = new Issue2167(self::SAMPLE);

        $this->assertSame(
            self::EXPECTED_OUTPUT,
            $sut->reduceWithClosureWrapper()
        );
    }
}
