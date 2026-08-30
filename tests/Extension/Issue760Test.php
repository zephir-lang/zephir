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
use Stub\Issue760;
use stdClass;
use Throwable;

/**
 * String offsets must behave exactly as PHP's do.
 *
 * PHP is the oracle here, so every case runs the same operation twice -- once
 * through the compiled extension, once in plain PHP -- and compares the whole
 * transcript: the diagnostics raised and the resulting value. Hard-coding the
 * expected text instead would pin down one PHP version; PHP changed its
 * string-offset diagnostics in 8.1 and again in 8.5, and this survives that.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/760
 */
final class Issue760Test extends TestCase
{
    private const SUBJECT = 'abcdef';

    private Issue760 $test;

    protected function setUp(): void
    {
        $this->test = new Issue760();
    }

    /**
     * @dataProvider intOffsetProvider
     */
    public function testReadWithNativeOffset(int $offset): void
    {
        $s = self::SUBJECT;

        $this->assertMatchesPhp(
            fn () => $this->test->dynIntOffset($s, $offset),
            fn () => $s[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->strIntOffset($s, $offset),
            fn () => $s[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->localStringOffset($offset),
            fn () => self::SUBJECT[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->returnStrOffset($s, $offset),
            fn () => $s[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->returnDynOffset($s, $offset),
            fn () => $s[$offset]
        );
    }

    /**
     * A native `char` destination takes the byte rather than a 1-char string:
     * `char` is int-like by construction. Out of range it stays NUL, but the
     * diagnostic still has to match.
     *
     * @dataProvider intOffsetProvider
     */
    public function testReadIntoNativeChar(int $offset): void
    {
        $s = self::SUBJECT;

        // ord('') is deprecated in 8.5, so unwrap the empty case by hand and
        // keep the transcript to the offset diagnostic alone.
        $php = static function () use ($s, $offset) {
            $character = $s[$offset];

            return '' === $character ? 0 : ord($character);
        };

        $this->assertMatchesPhp(fn () => $this->test->charDest($s, $offset), $php);
        $this->assertMatchesPhp(fn () => $this->test->dynCharDest($s, $offset), $php);
    }

    /**
     * @dataProvider offsetProvider
     */
    public function testReadWithDynamicOffset(mixed $offset): void
    {
        $s = self::SUBJECT;

        $this->assertMatchesPhp(
            fn () => $this->test->dynVarOffset($s, $offset),
            fn () => $s[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->strVarOffset($s, $offset),
            fn () => $s[$offset]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->literalOffset($offset),
            fn () => self::SUBJECT[$offset]
        );
    }

    /**
     * @dataProvider offsetProvider
     */
    public function testIssetAndEmpty(mixed $offset): void
    {
        $s = self::SUBJECT;

        $this->assertMatchesPhp(fn () => $this->test->dynIsset($s, $offset), fn () => isset($s[$offset]));
        $this->assertMatchesPhp(fn () => $this->test->strIsset($s, $offset), fn () => isset($s[$offset]));
        $this->assertMatchesPhp(fn () => $this->test->dynEmpty($s, $offset), fn () => empty($s[$offset]));
        $this->assertMatchesPhp(fn () => $this->test->strEmpty($s, $offset), fn () => empty($s[$offset]));
    }

    /**
     * '0' is the one in-range byte PHP calls empty.
     */
    public function testEmptyOnZeroByte(): void
    {
        $s = 'a0b';

        $this->assertMatchesPhp(fn () => $this->test->dynEmpty($s, 1), fn () => empty($s[1]));
        $this->assertMatchesPhp(fn () => $this->test->strEmpty($s, 1), fn () => empty($s[1]));
    }

    /**
     * `empty()` on a missing array key was raising "Undefined index", which
     * PHP does not do either.
     *
     * @dataProvider arrayOffsetProvider
     */
    public function testEmptyOnArrayIsSilent(mixed $offset): void
    {
        $a = [0 => 'a', 2 => 0];

        $this->assertMatchesPhp(
            fn () => $this->test->arrayEmpty($a, $offset),
            fn () => empty($a[$offset])
        );
    }

    /**
     * @dataProvider writeProvider
     */
    public function testWrite(mixed $offset, mixed $value): void
    {
        $s = self::SUBJECT;

        $php = static function () use ($s, $offset, $value) {
            $s[$offset] = $value;

            return $s;
        };

        $this->assertMatchesPhp(fn () => $this->test->dynWrite($s, $offset, $value), $php);
        $this->assertMatchesPhp(fn () => $this->test->strWrite($s, $offset, $value), $php);

        if (is_int($offset)) {
            $this->assertMatchesPhp(fn () => $this->test->dynWriteInt($s, $offset, $value), $php);
        }

        $this->assertMatchesPhp(
            fn () => $this->test->localStringWrite($offset, $value),
            static function () use ($offset, $value) {
                $local          = self::SUBJECT;
                $local[$offset] = $value;

                return $local;
            }
        );
    }

    /**
     * A string argument is passed by value, so the caller's copy must survive
     * a write untouched.
     */
    public function testWriteDoesNotReachTheCaller(): void
    {
        $this->assertMatchesPhp(
            function () {
                $caller = self::SUBJECT;
                $this->test->strWrite($caller, 0, 'Z');

                return $caller;
            },
            static function () {
                $caller = self::SUBJECT;
                (static function (string $copy) {
                    $copy[0] = 'Z';
                })($caller);

                return $caller;
            }
        );

        $this->assertMatchesPhp(
            function () {
                $caller = self::SUBJECT;
                $this->test->dynWrite($caller, 0, 'Z');

                return $caller;
            },
            static function () {
                $caller = self::SUBJECT;
                (static function ($copy) {
                    $copy[0] = 'Z';
                })($caller);

                return $caller;
            }
        );
    }

    public function testUnsetAndAppendThrow(): void
    {
        $s = self::SUBJECT;

        $this->assertMatchesPhp(
            fn () => $this->test->dynUnset($s, 0),
            static function () use ($s) {
                unset($s[0]);

                return $s;
            }
        );

        $this->assertMatchesPhp(
            fn () => $this->test->dynAppend($s),
            static function () use ($s) {
                $s[] = 'x';

                return $s;
            }
        );
    }

    /**
     * A non-string container must keep behaving exactly as it did.
     */
    public function testNonStringContainersAreUnaffected(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->dynIntOffset([7, 8], 1),
            static fn () => [7, 8][1]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->dynIntOffset('', 0),
            static fn () => ''[0]
        );
    }

    /**
     * `for c in s` is Zephir syntax with no PHP counterpart, so it is pinned
     * directly. A `var` value receives the 1-char strings a read would give,
     * a `char` value the raw bytes, and both source types agree.
     *
     * @dataProvider traverseProvider
     */
    public function testTraverse(string $subject, array $characters, array $bytes): void
    {
        $this->assertSame($characters, $this->test->dynTraverse($subject));
        $this->assertSame(array_reverse($characters), $this->test->dynTraverseReverse($subject));
        $this->assertSame($bytes, $this->test->strTraverse($subject));

        $keyed = [];
        foreach ($characters as $index => $character) {
            $keyed[] = [$index, $character];
        }

        $this->assertSame($keyed, $this->test->dynTraverseKeyed($subject));
    }

    public function testTraverseStillWalksArrays(): void
    {
        $this->assertSame([1, 2, 3], $this->test->dynTraverse([1, 2, 3]));
        $this->assertSame([[0, 1], [1, 2]], $this->test->dynTraverseKeyed([1, 2]));
        $this->assertSame([2, 1], $this->test->dynTraverseReverse([1, 2]));
    }

    public static function intOffsetProvider(): array
    {
        return [
            'first'          => [0],
            'middle'         => [2],
            'last'           => [5],
            'negative last'  => [-1],
            'negative first' => [-6],
            'past end'       => [6],
            'before start'   => [-7],
            'far past end'   => [100],
            'int max'        => [PHP_INT_MAX],
            'int min'        => [PHP_INT_MIN],
        ];
    }

    public static function offsetProvider(): array
    {
        return [
            'int'                    => [2],
            'negative int'           => [-1],
            'out of range int'       => [6],
            'numeric string'         => ['2'],
            'padded numeric string'  => ['  2 '],
            'leading zero string'    => ['02'],
            'signed string'          => ['+2'],
            'negative string'        => ['-2'],
            'leading numeric string' => ['2abc'],
            'non numeric string'     => ['abc'],
            'float string'           => ['1.5'],
            'overflowing string'     => ['99999999999999999999'],
            'float'                  => [1.5],
            'negative float'         => [-0.8],
            'huge float'             => [10e120],
            'nan'                    => [NAN],
            'inf'                    => [INF],
            'null'                   => [null],
            'false'                  => [false],
            'true'                   => [true],
            'array'                  => [[1, 2]],
            'object'                 => [new stdClass()],
        ];
    }

    public static function arrayOffsetProvider(): array
    {
        return [[0], [1], [2], [9], ['k']];
    }

    public static function writeProvider(): array
    {
        return [
            'in range'        => [1, 'Z'],
            'first'           => [0, 'Z'],
            'last'            => [5, 'Z'],
            'negative'        => [-1, 'Z'],
            'negative first'  => [-6, 'Z'],
            'negative oob'    => [-7, 'Z'],
            'past end'        => [10, 'Z'],
            'far past end'    => [20, 'N'],
            'empty value'     => [1, ''],
            'multibyte value' => [1, 'ZZ'],
            'int value'       => [1, 12],
            'float value'     => [1, 1.5],
            'null value'      => [1, null],
            'bool value'      => [1, true],
            'array value'     => [1, [1]],
            'numeric offset'  => ['2', 'Z'],
            'trailing offset' => ['2abc', 'Z'],
            'bad offset'      => ['abc', 'Z'],
            'float offset'    => [1.5, 'Z'],
            'null offset'     => [null, 'Z'],
            'array offset'    => [[1], 'Z'],
        ];
    }

    public static function traverseProvider(): array
    {
        return [
            'ascii'  => ['abc', ['a', 'b', 'c'], [97, 98, 99]],
            'empty'  => ['', [], []],
            'nul'    => ["a\0b", ['a', "\0", 'b'], [97, 0, 98]],
        ];
    }

    /**
     * Runs both sides and asserts the diagnostics and the value agree.
     */
    private function assertMatchesPhp(callable $extension, callable $php): void
    {
        $this->assertSame(
            $this->transcript($php),
            $this->transcript($extension),
            'The extension must behave exactly as PHP does'
        );
    }

    private function transcript(callable $subject): string
    {
        $diagnostics = [];

        set_error_handler(static function (int $code, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $message;

            return true;
        });

        try {
            $value  = $subject();
            $result = is_string($value)
                ? 'string(' . strlen($value) . ') ' . bin2hex($value)
                : var_export($value, true);
        } catch (Throwable $e) {
            $result = get_class($e) . ': ' . $e->getMessage();
        } finally {
            restore_error_handler();
        }

        return implode(' ; ', $diagnostics) . ' => ' . $result;
    }
}
