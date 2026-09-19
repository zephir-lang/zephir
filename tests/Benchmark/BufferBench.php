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

namespace Benchmark;

use PhpBench\Attributes as Bench;
use Stub\Bench as Workload;
use Stub\Buffer;

/**
 * Reading and writing a <Ns>\Buffer element by element, against the same loop
 * over a PHP array.
 *
 * The buffer subjects are the point of the kernel fast path in
 * kernel/array.c: without it each `buf[i]` is an offsetGet()/offsetSet()
 * method call, which is roughly an order of magnitude more work than a hash
 * lookup, let alone than the direct scalar load the fast path does.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(200)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class BufferBench
{
    private Workload $stub;
    private Buffer $buffer;
    private array $array;
    private int $elements = 1000;

    public function setUp(): void
    {
        $this->stub  = new Workload();
        $this->array = [];

        for ($i = 0; $i < $this->elements; ++$i) {
            $this->array[] = $i / 3.0;
        }

        $this->buffer = Buffer::fromArray($this->array);
    }

    public function benchZephirBufferSum(): void
    {
        $this->stub->bufferSumLoop($this->buffer, $this->elements);
    }

    public function benchZephirArraySum(): void
    {
        $this->stub->bufferSumOverArray($this->array, $this->elements);
    }

    public function benchPhpArraySum(): void
    {
        $total = 0.0;
        for ($i = 0; $i < $this->elements; ++$i) {
            $total += $this->array[$i];
        }
    }

    public function benchPhpBufferSum(): void
    {
        $total = 0.0;
        for ($i = 0; $i < $this->elements; ++$i) {
            $total += $this->buffer[$i];
        }
    }

    public function benchZephirBufferWrite(): void
    {
        $this->stub->bufferWriteLoop($this->buffer, $this->elements);
    }

    /**
     * Materialising the PHP array is the cost a Buffer exists to avoid paying
     * per operation; keep it visible next to the loops.
     */
    public function benchBufferToArray(): void
    {
        $this->buffer->toArray();
    }

    public function benchBufferFromArray(): void
    {
        Buffer::fromArray($this->array);
    }
}
