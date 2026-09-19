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
use Stub\Buffer;
use Stub\BufferOps;

/**
 * `buf[i]` written in Zephir compiles to a zephir_array_* kernel call, which
 * takes a direct fast path for a Buffer instead of dispatching offsetGet() /
 * offsetSet(). That is purely a speed change, so every assertion here is an
 * equivalence one: the compiled path and the PHP path must be
 * indistinguishable, including when the offset is bad.
 */
final class BufferZephirTest extends TestCase
{
    use AssertsPhpParity;

    private BufferOps $ops;

    protected function setUp(): void
    {
        $this->ops = new BufferOps();
    }

    public function testReadFromZephirMatchesReadFromPhp(): void
    {
        $buffer = Buffer::fromArray([1.5, 2.5, 3.5]);

        foreach ([0, 1, 2] as $index) {
            $this->assertSame($buffer[$index], $this->ops->readAt($buffer, $index));
        }
    }

    public function testReadFromZephirMatchesReadFromPhpForLongs(): void
    {
        $buffer = Buffer::fromArray([10, 20, 30], Buffer::TYPE_LONG);

        foreach ([0, 1, 2] as $index) {
            $this->assertSame($buffer[$index], $this->ops->readAt($buffer, $index));
        }
    }

    public function testWriteFromZephirIsVisibleFromPhp(): void
    {
        $buffer = new Buffer(3);

        $this->ops->writeAt($buffer, 1, 7.25);

        $this->assertSame([0.0, 7.25, 0.0], $buffer->toArray());
    }

    public function testWriteFromZephirCoercesLikeAPhpWrite(): void
    {
        $fromZephir = new Buffer(1);
        $fromPhp    = new Buffer(1);

        $this->ops->writeAt($fromZephir, 0, '2.5');
        $fromPhp[0] = '2.5';

        $this->assertSame($fromPhp->toArray(), $fromZephir->toArray());
    }

    public function testIssetFromZephirMatchesIssetFromPhp(): void
    {
        $buffer = Buffer::fromArray([1.0, 2.0]);

        foreach ([0, 1, 2, -1] as $index) {
            $this->assertSame(isset($buffer[$index]), $this->ops->has($buffer, $index));
        }
    }

    public function testCountFromZephirMatchesCountFromPhp(): void
    {
        foreach ([0, 1, 17] as $size) {
            $buffer = new Buffer($size);

            $this->assertSame(count($buffer), $this->ops->size($buffer));
        }
    }

    /**
     * @dataProvider badOffsets
     */
    public function testOutOfRangeReadFromZephirRaisesWhatPhpRaises(int $index): void
    {
        $this->assertMatchesPhp(
            fn () => $this->ops->readAt(Buffer::fromArray([1.0, 2.0]), $index),
            static function () use ($index) {
                $buffer = Buffer::fromArray([1.0, 2.0]);

                return $buffer[$index];
            },
        );
    }

    /**
     * @dataProvider badOffsets
     */
    public function testOutOfRangeWriteFromZephirRaisesWhatPhpRaises(int $index): void
    {
        $this->assertMatchesPhp(
            function () use ($index) {
                $this->ops->writeAt(Buffer::fromArray([1.0, 2.0]), $index, 9.0);

                return 'written';
            },
            static function () use ($index) {
                $buffer         = Buffer::fromArray([1.0, 2.0]);
                $buffer[$index] = 9.0;

                return 'written';
            },
        );
    }

    /**
     * @return array<string, array{0: int}>
     */
    public static function badOffsets(): array
    {
        return [
            'past the end' => [5],
            'negative'     => [-1],
        ];
    }

    public function testASumLoopWrittenInZephirAgreesWithPhp(): void
    {
        $values = [];
        for ($i = 0; $i < 64; ++$i) {
            $values[] = $i / 3.0;
        }

        $buffer = Buffer::fromArray($values);

        $this->assertSame(array_sum($values), $this->ops->sum($buffer));
    }

    public function testAWriteLoopWrittenInZephirAgreesWithPhp(): void
    {
        $values = [1.0, 2.0, 3.0, 4.0];
        $buffer = Buffer::fromArray($values);

        $this->ops->scale($buffer, 2.5);

        $this->assertSame(array_map(static fn ($v) => $v * 2.5, $values), $buffer->toArray());
    }

    /**
     * The fast path must not change the lifetime of anything: a read builds a
     * fresh scalar zval, a write consumes one, and neither owns the buffer.
     */
    public function testTheZephirPathDoesNotGrowTheHeap(): void
    {
        $buffer = Buffer::fromArray([1.0, 2.0, 3.0, 4.0]);

        for ($i = 0; $i < 200; ++$i) {
            $this->ops->sum($buffer);
            $this->ops->scale($buffer, 1.0);
        }

        $before = memory_get_usage();

        for ($i = 0; $i < 2000; ++$i) {
            $this->ops->sum($buffer);
            $this->ops->scale($buffer, 1.0);
        }

        $this->assertSame($before, memory_get_usage());
    }
}
