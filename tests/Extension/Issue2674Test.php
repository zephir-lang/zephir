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
use Stub\Issue2674;

/**
 * PHP's `explode()` reads the limit three ways: `limit > 1` splits and merges
 * the tail, `limit < 0` drops the last `abs(limit)` pieces, and a limit of 0 or
 * 1 returns the subject whole. Zephir passed every limit to `php_explode()`,
 * which implements only the first of the three, so anything `<= 1` came back as
 * the `limit == 2` result. An empty separator is a `ValueError` in PHP and was
 * an endless loop here.
 *
 * PHP itself is the oracle rather than a table of expected arrays: the
 * `ValueError` message is "cannot be empty" on PHP 8.0 to 8.3 and "must not be
 * empty" from 8.4, so a hard-coded string would pin these assertions to one
 * leg of the matrix.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2674
 */
final class Issue2674Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * Every shape the subject can take around a single-character separator:
     * absent, leading, trailing, doubled, and the plain case.
     */
    private const SUBJECTS = [
        'four pieces'        => 'a,b,c,d',
        'two pieces'         => 'a,b',
        'single piece'       => 'abcd',
        'empty subject'      => '',
        'only separator'     => ',',
        'only separators'    => ',,',
        'leading separator'  => ',a',
        'trailing separator' => 'a,',
        'empty middle piece' => 'a,,b',
    ];

    /**
     * The three branches plus their boundaries, and both saturation points.
     */
    private const LIMITS = [PHP_INT_MIN, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, PHP_INT_MAX];

    private Issue2674 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2674();
    }

    public static function commaProvider(): array
    {
        $cases = [];

        foreach (self::SUBJECTS as $name => $subject) {
            foreach (self::LIMITS as $limit) {
                $cases[$name . ', limit ' . $limit] = [$subject, $limit];
            }
        }

        return $cases;
    }

    /**
     * A separator longer than one byte takes a different path through
     * `zend_memnstr()`, and the chunk arithmetic has to subtract its length.
     */
    public static function multiCharProvider(): array
    {
        $subjects = [
            'four pieces'        => 'a::b::c::d',
            'single piece'       => 'abcd',
            'empty subject'      => '',
            'only separator'     => '::',
            'trailing separator' => 'a::',
            'partial separator'  => 'a:b',
        ];

        $cases = [];
        foreach ($subjects as $name => $subject) {
            foreach (self::LIMITS as $limit) {
                $cases[$name . ', limit ' . $limit] = ['::', $subject, $limit];
            }
        }

        return $cases;
    }

    public static function subjectProvider(): array
    {
        $cases = [];
        foreach (self::SUBJECTS as $name => $subject) {
            $cases[$name] = [$subject];
        }

        return $cases;
    }

    /**
     * Literal separator, runtime limit: `zephir_fast_explode_str()`.
     *
     * @dataProvider commaProvider
     */
    public function testLiteralSeparatorMatchesPhp(string $subject, int $limit): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeStrLimit($subject, $limit),
            fn () => explode(',', $subject, $limit)
        );
    }

    /**
     * Runtime separator, runtime limit: `zephir_fast_explode()`.
     *
     * @dataProvider commaProvider
     */
    public function testRuntimeSeparatorMatchesPhp(string $subject, int $limit): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit(',', $subject, $limit),
            fn () => explode(',', $subject, $limit)
        );
    }

    /**
     * @dataProvider multiCharProvider
     */
    public function testMultiCharSeparatorMatchesPhp(string $separator, string $subject, int $limit): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit($separator, $subject, $limit),
            fn () => explode($separator, $subject, $limit)
        );
    }

    /**
     * The optimizer folds a constant limit into the generated C instead of
     * routing it through `zephir_get_intval()`.
     *
     * @dataProvider subjectProvider
     */
    public function testConstantLimitMatchesPhp(string $subject): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeConstLimit($subject),
            fn () => explode(',', $subject, -2)
        );
    }

    /**
     * An omitted limit, where the optimizer supplies `ZEND_LONG_MAX`.
     *
     * @dataProvider subjectProvider
     */
    public function testDefaultLimitMatchesPhp(string $subject): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeNoLimit(',', $subject),
            fn () => explode(',', $subject)
        );
    }

    /**
     * `zend_memnstr()` returns the haystack, not NULL, for a zero-length
     * needle, so `php_explode()` advanced by nothing and looped until the
     * allocator gave up.
     */
    public function testEmptySeparatorThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeNoLimit('', 'a,b'),
            fn () => explode('', 'a,b')
        );
    }

    /**
     * @dataProvider limitProvider
     */
    public function testEmptySeparatorThrowsLikePhpAtEveryLimit(int $limit): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit('', 'a,b', $limit),
            fn () => explode('', 'a,b', $limit)
        );
    }

    public static function limitProvider(): array
    {
        $cases = [];
        foreach (self::LIMITS as $limit) {
            $cases['limit ' . $limit] = [$limit];
        }

        return $cases;
    }
}
