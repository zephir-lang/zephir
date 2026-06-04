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
use Stub\UnreachableLoop;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1170
 */
final class UnreachableLoopTest extends TestCase
{
    public function testGuardRunsOnLaterIterations(): void
    {
        // The `if x` guard (x flips to true after the first iteration) must
        // execute on later iterations, joining entries with ", ".
        $this->assertSame(
            '{a: "1", b: "2"}',
            UnreachableLoop::toPropertyString(['a' => '1', 'b' => '2'])
        );
    }

    public function testSingleEntryHasNoSeparator(): void
    {
        $this->assertSame(
            '{a: "1"}',
            UnreachableLoop::toPropertyString(['a' => '1'])
        );
    }

    public function testEmptyProperties(): void
    {
        $this->assertSame('{}', UnreachableLoop::toPropertyString([]));
    }
}
