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
use Stub\Issue1097;

/**
 * Regression test for https://github.com/zephir-lang/zephir/issues/1097
 *
 * elseif conditions containing sub-expressions with side effects (array element
 * access, property access, etc.) must not be evaluated when an earlier if/elseif
 * branch has already been taken. The compiler was hoisting the preamble code
 * (zephir_array_fetch_long and similar) unconditionally before the outer if,
 * triggering PHP Notices such as "Cannot use a scalar value as an array".
 *
 * Fix: preamble code is now emitted inside its owning else-branch so it only
 * runs when that branch is actually reached.
 */
final class Issue1097Test extends TestCase
{
    private Issue1097 $obj;

    protected function setUp(): void
    {
        $this->obj = new Issue1097();
    }

    /**
     * Passing a string must return "string" and must NOT trigger any
     * "Cannot use a scalar value as an array" notice. This is the canonical
     * regression from the bug report: before the fix the array-fetch preamble
     * was hoisted unconditionally before the outer if, so a notice fired even
     * though the if-branch short-circuited with return.
     */
    public function testStringInputSkipsElseif(): void
    {
        // Install a strict error handler so any unexpected notice/warning turns
        // into a test failure.
        $prev = set_error_handler(static function (int $code, string $message): bool {
            throw new \ErrorException($message, 0, $code);
        });

        try {
            $result = $this->obj->testStringOrArrayOfString('hello');
        } finally {
            set_error_handler($prev);
        }

        $this->assertSame('string', $result);
    }

    /**
     * Passing an array whose first element is a string must return
     * "array-of-string" (the elseif branch is reached correctly).
     */
    public function testArrayOfStringInput(): void
    {
        $result = $this->obj->testStringOrArrayOfString(['hello', 'world']);

        $this->assertSame('array-of-string', $result);
    }

    /**
     * Passing an integer falls through to "other". The elseif condition
     * (myvar[0] on a scalar) will trigger a PHP notice/warning — that is
     * correct PHP-compatible behaviour and mirrors what `is_string(42[0])`
     * does in plain PHP. The key point is that this notice now fires only
     * inside the else-branch (not unconditionally before the outer if).
     */
    public function testIntegerInputFallsThrough(): void
    {
        // Suppress the expected PHP notice for scalar[0] access so we can
        // assert the return value without PHPUnit converting it to an error.
        $result = null;
        $prev   = set_error_handler(static function () {
            return true; // absorb the "Cannot use a scalar value as an array" notice
        });

        try {
            $result = $this->obj->testStringOrArrayOfString(42);
        } finally {
            set_error_handler($prev);
        }

        $this->assertSame('other', $result);
    }

    /**
     * Passing a string with an else branch — the else must remain unreachable.
     */
    public function testWithElseStringInput(): void
    {
        $this->assertSame('string', $this->obj->testWithElse('hello'));
    }

    /**
     * Array input hits the elseif (is_array), the else is not reached.
     */
    public function testWithElseArrayInput(): void
    {
        $this->assertSame('array', $this->obj->testWithElse(['a', 'b']));
    }

    /**
     * Integer input hits neither if nor elseif, so the else branch fires.
     */
    public function testWithElseOtherInput(): void
    {
        $this->assertSame('other', $this->obj->testWithElse(99));
    }

    /**
     * Two elseif branches: string input must return immediately without
     * evaluating either elseif condition (and therefore without any notice).
     */
    public function testMultipleElseifStringInput(): void
    {
        $prev = set_error_handler(static function (int $code, string $message): bool {
            throw new \ErrorException($message, 0, $code);
        });

        try {
            $result = $this->obj->testMultipleElseif('hello');
        } finally {
            set_error_handler($prev);
        }

        $this->assertSame('string', $result);
    }

    /**
     * Two elseif branches: array-of-string matches the first elseif and must
     * not reach the second elseif (even though it would also pass is_int check
     * on an unrelated value).
     */
    public function testMultipleElseifArrayOfString(): void
    {
        $result = $this->obj->testMultipleElseif(['hello']);

        $this->assertSame('array-of-string', $result);
    }

    /**
     * Two elseif branches: array-of-int matches the second elseif.
     */
    public function testMultipleElseifArrayOfInt(): void
    {
        $result = $this->obj->testMultipleElseif([42]);

        $this->assertSame('array-of-int', $result);
    }

    /**
     * Two elseif branches: integer input falls through to "other". Each
     * elseif will still attempt myvar[0] on a scalar and produce a PHP
     * notice (expected PHP-compatible behaviour). The critical invariant is
     * that these notices fire inside the else-branches, not unconditionally
     * before the outer if.
     */
    public function testMultipleElseifIntegerFallsThrough(): void
    {
        $result = null;
        $prev   = set_error_handler(static function () {
            return true;
        });

        try {
            $result = $this->obj->testMultipleElseif(99);
        } finally {
            set_error_handler($prev);
        }

        $this->assertSame('other', $result);
    }

    /**
     * Simple elseif (no sub-expression preamble) must be unaffected by the fix.
     */
    public function testSimpleElseifPositive(): void
    {
        $this->assertSame('positive', $this->obj->testSimpleElseif(5));
    }

    public function testSimpleElseifZero(): void
    {
        $this->assertSame('zero', $this->obj->testSimpleElseif(0));
    }

    public function testSimpleElseifNegative(): void
    {
        $this->assertSame('negative', $this->obj->testSimpleElseif(-3));
    }
}
