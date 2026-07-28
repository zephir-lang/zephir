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

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\Substr;

final class SubstrTest extends TestCase
{
    public function testTwoArguments1(): void
    {
        $test = new Substr();

        $strings_array = [null, '', 12345, 'abcdef', '123abc', '_123abc'];
        $results1 = ['', '', '2345', 'bcdef', '23abc', '123abc'];
        $results2 = ['', '', '12345', 'abcdef', '123abc', '_123abc'];
        $results3 = ['', '', '45', 'ef', 'bc', 'bc'];

        $c = 0;
        foreach ($strings_array as $str) {
            $this->assertSame($test->testTwoArguments($str, 1), $results1[$c]);
            $this->assertSame($test->testTwoArguments($str, 0), $results2[$c]);
            $this->assertSame($test->testTwoArguments($str, -2), $results3[$c]);
            ++$c;
        }
    }

    public function testThreeArguments1(): void
    {
        $test = new Substr();
        $this->assertSame($test->testThreeArguments('abcdef', 0, -3), 'abc');
        $this->assertSame($test->testThreeArguments('123abc', 0, 3), '123');
        $this->assertSame($test->testThreeArguments('abcdef', 1, -3), 'bc');
        $this->assertSame($test->testThreeArguments('abcdef', -2, 0), '');
        $this->assertSame($test->testThreeArguments(12345, 1, -1), '234');
    }

    /**
     * An offset equal to the string length is in range for every PHP version
     * ever released - it yields "". The kernel used to reject it with `false`
     * because of an off-by-one (`f >= str_len`) in the 2015 kernel port.
     *
     * @see https://github.com/zephir-lang/zephir/blob/master/kernel/string.c
     */
    public function testOffsetAtStringLengthReturnsEmptyString(): void
    {
        $test = new Substr();

        $this->assertSame($test->testTwoArguments('GetPosts', 8), '');
        $this->assertSame($test->testTwoArguments('abcdef', 6), '');
        $this->assertSame($test->testThreeArguments('abcdef', 6, 2), '');
    }

    /**
     * PHP 8.0 replaced substr()'s out-of-range `false` return with clamping.
     */
    public function testOffsetPastStringLengthReturnsEmptyString(): void
    {
        $test = new Substr();

        $this->assertSame($test->testTwoArguments('GetPosts', 9), '');
        $this->assertSame($test->testTwoArguments('GetPosts', 100), '');
        $this->assertSame($test->testThreeArguments('GetPosts', 100, 3), '');
    }

    /**
     * The empty string is an ordinary runtime value, not an edge case: every
     * offset into it must yield "" rather than `false`.
     */
    public function testEmptySubjectAlwaysReturnsEmptyString(): void
    {
        $test = new Substr();

        $this->assertSame($test->testTwoArguments('', 0), '');
        $this->assertSame($test->testTwoArguments('', 1), '');
        $this->assertSame($test->testTwoArguments('', -2), '');
        $this->assertSame($test->testThreeArguments('', 0, 5), '');
    }

    /**
     * A negative length larger than the subject clamps to zero.
     */
    public function testNegativeLengthLargerThanSubjectReturnsEmptyString(): void
    {
        $test = new Substr();

        $this->assertSame($test->testThreeArguments('abcdef', 0, -10), '');
        $this->assertSame($test->testThreeArguments('abcdef', 2, -10), '');
        $this->assertSame($test->testThreeArguments('abcdef', -2, -10), '');
    }

    /**
     * Offsets are clamped, so the integer extremes must not overflow or fail.
     */
    public function testIntegerExtremesAreClamped(): void
    {
        $test = new Substr();

        $this->assertSame($test->testTwoArguments('abcdef', PHP_INT_MIN), 'abcdef');
        $this->assertSame($test->testTwoArguments('abcdef', PHP_INT_MAX), '');
        $this->assertSame($test->testThreeArguments('abcdef', 0, PHP_INT_MAX), 'abcdef');
        $this->assertSame($test->testThreeArguments('abcdef', 0, PHP_INT_MIN), '');
    }

    /**
     * Scalars are coerced the way PHP coerces them, rather than rejected.
     */
    public function testScalarSubjectsAreCoerced(): void
    {
        $test = new Substr();

        $this->assertSame($test->testTwoArguments(true, 0), '1');
        $this->assertSame($test->testTwoArguments(false, 0), '');
        $this->assertSame($test->testTwoArguments(null, 0), '');
        $this->assertSame($test->testTwoArguments(1.5, 0), '1.5');
    }
}
