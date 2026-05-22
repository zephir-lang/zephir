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
 * Measures repeated string concatenation. Exercises ZEPHIR_CONCAT_* macros.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class StringConcatBench
{
    private Workload $stub;
    private string $base = 'abc';
    private int $iterations = 50;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchZephirStringConcat(): void
    {
        $this->stub->stringConcatLoop($this->base, $this->iterations);
    }

    public function benchPhpStringConcat(): void
    {
        $out = '';
        for ($i = 0; $i < $this->iterations; $i++) {
            $out = $out . $this->base;
        }
    }
}
