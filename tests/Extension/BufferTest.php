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
use SplFixedArray;
use Stub\Buffer;
use Throwable;

use function get_class;
use function str_replace;

/**
 * <Ns>\Buffer -- the contiguous C-backed numeric buffer of issue #2721.
 *
 * SplFixedArray is the oracle: it is the closest thing PHP ships (a fixed-size,
 * integer-indexed, ArrayAccess + Countable container) and its offset semantics
 * moved three times between 8.0 and 8.5 -- the out-of-range exception class
 * changed from RuntimeException to OutOfBoundsException in 8.4, and the
 * illegal-offset message changed in 8.1 and again in 8.3. Comparing two live
 * evaluations rather than hard-coded strings is the only assertion that
 * survives all of it.
 */
final class BufferTest extends TestCase
{
    use AssertsPhpParity;

    public function testConstructZeroFillsADoubleBuffer(): void
    {
        $buffer = new Buffer(3);

        $this->assertSame(Buffer::TYPE_DOUBLE, $buffer->type());
        $this->assertCount(3, $buffer);
        $this->assertSame([0.0, 0.0, 0.0], $buffer->toArray());
    }

    public function testConstructZeroFillsALongBuffer(): void
    {
        $buffer = new Buffer(3, Buffer::TYPE_LONG);

        $this->assertSame(Buffer::TYPE_LONG, $buffer->type());
        $this->assertCount(3, $buffer);
        $this->assertSame([0, 0, 0], $buffer->toArray());
    }

    public function testEmptyBufferIsAllowed(): void
    {
        $buffer = new Buffer(0);

        $this->assertCount(0, $buffer);
        $this->assertSame([], $buffer->toArray());
    }

    public function testFromArrayRoundTripsDoubles(): void
    {
        $buffer = Buffer::fromArray([1.5, -2.25, 0.0]);

        $this->assertSame(Buffer::TYPE_DOUBLE, $buffer->type());
        $this->assertSame([1.5, -2.25, 0.0], $buffer->toArray());
    }

    public function testFromArrayRoundTripsLongs(): void
    {
        $buffer = Buffer::fromArray([1, -2, 0], Buffer::TYPE_LONG);

        $this->assertSame(Buffer::TYPE_LONG, $buffer->type());
        $this->assertSame([1, -2, 0], $buffer->toArray());
    }

    /**
     * A Buffer is positional, so a hashed source array contributes its values
     * in iteration order and its keys are discarded -- the same thing
     * SplFixedArray::fromArray() does with $preserveKeys = false.
     */
    public function testFromArrayTakesHashedInputInIterationOrder(): void
    {
        $buffer = Buffer::fromArray(['b' => 2.0, 'a' => 1.0, 7 => 3.0]);

        $this->assertSame([2.0, 1.0, 3.0], $buffer->toArray());
    }

    public function testFromArrayCoercesToTheElementType(): void
    {
        $this->assertSame([1.0, 2.0], Buffer::fromArray([1, '2'])->toArray());
        $this->assertSame([1, 2], Buffer::fromArray([1.9, '2.9'], Buffer::TYPE_LONG)->toArray());
    }

    public function testOffsetGetAndOffsetSet(): void
    {
        $buffer = new Buffer(2);

        $buffer[0] = 1.5;
        $buffer[1] = -2.5;

        $this->assertSame(1.5, $buffer[0]);
        $this->assertSame(-2.5, $buffer[1]);
    }

    public function testOffsetSetOnALongBufferStoresIntegers(): void
    {
        $buffer = new Buffer(1, Buffer::TYPE_LONG);

        $buffer[0] = 42;

        $this->assertSame(42, $buffer[0]);
    }

    public function testFillOverwritesEveryElement(): void
    {
        $buffer = Buffer::fromArray([1.0, 2.0, 3.0]);

        $buffer->fill(7.5);

        $this->assertSame([7.5, 7.5, 7.5], $buffer->toArray());
    }

    public function testNegativeSizeIsRejected(): void
    {
        $this->assertMatchesPhp(
            $this->renamed(static fn () => new Buffer(-1)),
            static fn () => new SplFixedArray(-1),
        );
    }

    public function testUnknownElementTypeIsRejected(): void
    {
        $this->expectException(\ValueError::class);

        /** @phpstan-ignore-next-line intentionally invalid */
        new Buffer(1, 99);
    }

    /**
     * @dataProvider illegalOrOutOfRangeOffsets
     *
     * @param mixed $offset
     */
    public function testReadingABadOffsetBehavesAsSplFixedArrayDoes($offset): void
    {
        $this->assertMatchesPhp(
            $this->renamed(static function () use ($offset) {
                $buffer = Buffer::fromArray([10.0, 11.0, 12.0]);

                return $buffer[$offset];
            }),
            static function () use ($offset) {
                $fixed = SplFixedArray::fromArray([10.0, 11.0, 12.0]);

                return $fixed[$offset];
            },
        );
    }

    /**
     * @dataProvider illegalOrOutOfRangeOffsets
     *
     * @param mixed $offset
     */
    public function testWritingABadOffsetBehavesAsSplFixedArrayDoes($offset): void
    {
        $this->assertMatchesPhp(
            $this->renamed(static function () use ($offset) {
                $buffer          = Buffer::fromArray([10.0, 11.0, 12.0]);
                $buffer[$offset] = 1.0;

                return 'written';
            }),
            static function () use ($offset) {
                $fixed          = SplFixedArray::fromArray([10.0, 11.0, 12.0]);
                $fixed[$offset] = 1.0;

                return 'written';
            },
        );
    }

    /**
     * @dataProvider illegalOrOutOfRangeOffsets
     *
     * @param mixed $offset
     */
    public function testIssetOnABadOffsetBehavesAsSplFixedArrayDoes($offset): void
    {
        $this->assertMatchesPhp(
            $this->renamed(static function () use ($offset) {
                $buffer = Buffer::fromArray([10.0, 11.0, 12.0]);

                return isset($buffer[$offset]);
            }),
            static function () use ($offset) {
                $fixed = SplFixedArray::fromArray([10.0, 11.0, 12.0]);

                return isset($fixed[$offset]);
            },
        );
    }

    /**
     * @return array<string, array{0: mixed}>
     */
    public static function illegalOrOutOfRangeOffsets(): array
    {
        return [
            'past the end'      => [5],
            'negative'          => [-1],
            'numeric string'    => ['1'],
            'text string'       => ['x'],
            'float'             => [1.7],
            'null'              => [null],
            'true'              => [true],
            'false'             => [false],
        ];
    }

    public function testAppendingIsRejectedAsSplFixedArrayRejectsIt(): void
    {
        $this->assertMatchesPhp(
            $this->renamed(static function () {
                $buffer   = new Buffer(2);
                $buffer[] = 1.0;

                return 'appended';
            }),
            static function () {
                $fixed   = new SplFixedArray(2);
                $fixed[] = 1.0;

                return 'appended';
            },
        );
    }

    /**
     * An element is a raw C scalar, so there is no zval to hand back as an
     * lvalue. That is the ordinary situation for an overloaded element, and
     * PHP already has a defined behaviour for it -- a notice, and the write is
     * dropped. Assert it against a plain ArrayAccess class rather than against
     * SplFixedArray, whose elements *are* zvals and so can be modified in place.
     */
    public function testCompoundAssignmentBehavesAsAnyOverloadedElementDoes(): void
    {
        $overloaded = new class implements \ArrayAccess {
            /** @var array<int, float> */
            public array $values = [0 => 10.0];

            public function offsetExists($offset): bool
            {
                return isset($this->values[$offset]);
            }

            #[\ReturnTypeWillChange]
            public function offsetGet($offset)
            {
                return $this->values[$offset];
            }

            public function offsetSet($offset, $value): void
            {
                $this->values[$offset] = $value;
            }

            public function offsetUnset($offset): void
            {
                unset($this->values[$offset]);
            }
        };

        $this->assertMatchesPhp(
            static function () {
                $buffer = Buffer::fromArray([10.0]);
                $buffer[0] += 5.0;

                return $buffer[0];
            },
            static function () use ($overloaded) {
                $overloaded[0] += 5.0;

                return $overloaded[0];
            },
        );
    }

    public function testForeachYieldsEveryElementInOrder(): void
    {
        $seen = [];

        foreach (Buffer::fromArray([1.5, 2.5, 3.5]) as $index => $value) {
            $seen[] = "$index=$value";
        }

        $this->assertSame(['0=1.5', '1=2.5', '2=3.5'], $seen);
        $this->assertTrue(is_iterable(Buffer::fromArray([1.0])));
    }

    public function testJsonEncodeEmitsTheElementsNotAnEmptyObject(): void
    {
        $this->assertSame('[1.5,2.5]', json_encode(Buffer::fromArray([1.5, 2.5])));
        $this->assertSame('[1,2]', json_encode(Buffer::fromArray([1, 2], Buffer::TYPE_LONG)));
    }

    /**
     * The one place a Buffer cannot agree with SplFixedArray: SplFixedArray's
     * slots start out null, so isset() is false on a freshly constructed one.
     * A numeric buffer starts at zero and has no value that reads as absent.
     */
    public function testEveryInRangeElementIsSetEvenWhenZero(): void
    {
        $buffer = new Buffer(2);

        $this->assertTrue(isset($buffer[0]));
        $this->assertFalse(empty($buffer[1] ?? null) === false && $buffer[1] !== 0.0);
        $this->assertTrue(empty($buffer[0]), 'empty() still follows the value, as it does for arrays');
        $this->assertSame(0.0, $buffer[1] ?? 'missing');
        $this->assertSame('missing', $buffer[9] ?? 'missing');
    }

    public function testOffsetExistsIsTrueOnlyInRange(): void
    {
        $buffer = new Buffer(2);

        $this->assertTrue(isset($buffer[0]));
        $this->assertTrue(isset($buffer[1]));
        $this->assertFalse(isset($buffer[2]));
    }

    /**
     * null is not representable in a numeric buffer, so unset() writes the zero
     * element. SplFixedArray stores null there, which is the one place the two
     * cannot agree, so it is asserted directly rather than against the oracle.
     */
    public function testOffsetUnsetWritesTheZeroElement(): void
    {
        $buffer = Buffer::fromArray([1.5, 2.5]);

        unset($buffer[0]);

        $this->assertSame(0.0, $buffer[0]);
        $this->assertSame(2.5, $buffer[1]);
    }

    /**
     * @dataProvider coercibleValues
     *
     * @param mixed $value
     */
    public function testWritingADoubleBufferCoercesExactlyAsAFloatCastDoes($value): void
    {
        $this->assertMatchesPhp(
            static function () use ($value) {
                $buffer    = new Buffer(1);
                $buffer[0] = $value;

                return $buffer[0];
            },
            static fn () => (float) $value,
        );
    }

    /**
     * @dataProvider coercibleValues
     *
     * @param mixed $value
     */
    public function testWritingALongBufferCoercesExactlyAsAnIntCastDoes($value): void
    {
        $this->assertMatchesPhp(
            static function () use ($value) {
                $buffer    = new Buffer(1, Buffer::TYPE_LONG);
                $buffer[0] = $value;

                return $buffer[0];
            },
            static fn () => (int) $value,
        );
    }

    /**
     * @return array<string, array{0: mixed}>
     */
    public static function coercibleValues(): array
    {
        return [
            'int'                 => [7],
            'negative int'        => [-7],
            'float'               => [1.5],
            'float losing digits' => [3.9],
            'numeric string'      => ['2.5'],
            'integer string'      => ['2'],
            'leading numeric'     => ['12abc'],
            'non numeric string'  => ['abc'],
            'empty string'        => [''],
            'true'                => [true],
            'false'               => [false],
            'null'                => [null],
        ];
    }

    public function testCloneIsIndependent(): void
    {
        $original = Buffer::fromArray([1.0, 2.0]);
        $copy     = clone $original;

        $copy[0] = 99.0;

        $this->assertSame(1.0, $original[0], 'mutating the clone must not reach the original');
        $this->assertSame(99.0, $copy[0]);
    }

    public function testSerializeRoundTripPreservesTypeAndValues(): void
    {
        foreach ([Buffer::TYPE_DOUBLE, Buffer::TYPE_LONG] as $type) {
            $original = Buffer::fromArray([1, 2, 3], $type);

            /** @var Buffer $restored */
            $restored = unserialize(serialize($original));

            $this->assertSame($type, $restored->type());
            $this->assertSame($original->toArray(), $restored->toArray());
            $this->assertCount(3, $restored);
        }
    }

    public function testVarDumpShowsTheElements(): void
    {
        ob_start();
        var_dump(Buffer::fromArray([1.5, 2.5]));
        $dump = (string) ob_get_clean();

        $this->assertStringContainsString('1.5', $dump);
        $this->assertStringContainsString('2.5', $dump);
    }

    public function testTheClassIsFinalAndRejectsDynamicProperties(): void
    {
        $reflection = new \ReflectionClass(Buffer::class);

        $this->assertTrue($reflection->isFinal());

        $this->expectException(Throwable::class);

        $buffer = new Buffer(1);
        /** @phpstan-ignore-next-line intentionally invalid */
        $buffer->nope = 1;
    }

    /**
     * Valgrind is not a usable leak gate for a PHP extension here, so the probe
     * is an in-process loop: allocate nothing inside the measured window beyond
     * the subject itself, and require the arena not to grow.
     *
     * @dataProvider leakSubjects
     */
    public function testRepeatedUseDoesNotGrowTheHeap(callable $subject): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $subject();
        }

        $before = memory_get_usage();

        for ($i = 0; $i < 2000; ++$i) {
            $subject();
        }

        $this->assertSame($before, memory_get_usage());
    }

    /**
     * @return array<string, array{0: callable}>
     */
    public static function leakSubjects(): array
    {
        return [
            'construct and destroy' => [static function (): void {
                $buffer = new Buffer(64);
                unset($buffer);
            }],
            'fromArray'             => [static function (): void {
                $buffer = Buffer::fromArray([1.0, 2.0, 3.0, 4.0]);
                unset($buffer);
            }],
            'toArray'               => [static function (): void {
                $buffer = Buffer::fromArray([1.0, 2.0, 3.0, 4.0]);
                $buffer->toArray();
                unset($buffer);
            }],
            'clone'                 => [static function (): void {
                $buffer = Buffer::fromArray([1.0, 2.0, 3.0, 4.0]);
                $copy   = clone $buffer;
                unset($buffer, $copy);
            }],
            'offset write and read' => [static function (): void {
                $buffer    = new Buffer(4);
                $buffer[2] = 1.5;
                $buffer[2];
                unset($buffer);
            }],
            'failed offset'         => [static function (): void {
                $buffer = new Buffer(4);

                try {
                    $buffer[9];
                } catch (Throwable $e) {
                    // the throwing path must not leak either
                }

                unset($buffer);
            }],
        ];
    }

    /**
     * Wraps a subject so a thrown message naming Stub\Buffer reads as though it
     * named SplFixedArray. PHP puts the real class name in these messages
     * ("Cannot access offset of type string on SplFixedArray", 8.3+), so the
     * two sides *should* differ there and nowhere else.
     */
    private function renamed(callable $subject): callable
    {
        return static function () use ($subject) {
            try {
                return $subject();
            } catch (Throwable $e) {
                $class = get_class($e);

                throw new $class(str_replace(Buffer::class, SplFixedArray::class, $e->getMessage()));
            }
        };
    }
}
