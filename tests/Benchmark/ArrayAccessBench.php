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

/**
 * Measures repeated `arr["key"]` fetch by string key. Exercises
 * `zephir_array_fetch_string`.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class ArrayAccessBench
{
    private Workload $stub;
    private array $array;
    private int $iterations = 100;

    public function setUp(): void
    {
        $this->stub = new Workload();
        $this->array = ['a' => 1, 'b' => 2, 'c' => 3];
    }

    public function benchZephirArrayFetchString(): void
    {
        $this->stub->arrayFetchStringLoop($this->array, $this->iterations);
    }

    public function benchPhpArrayFetchString(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $sum += $this->array['a'] + $this->array['b'];
        }
    }
}
