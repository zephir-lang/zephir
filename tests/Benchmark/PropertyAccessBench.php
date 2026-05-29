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
 * Measures `this->prop` property reads in a hot loop. Exercises
 * `zephir_read_property` (the kernel helper most affected by string
 * literal handling, see #1902).
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class PropertyAccessBench
{
    private Workload $stub;
    private PhpPropertyHolder $php;
    private int $iterations = 100;

    public function setUp(): void
    {
        $this->stub = new Workload();
        $this->php = new PhpPropertyHolder();
    }

    public function benchZephirPropertyRead(): void
    {
        $this->stub->propertyReadLoop($this->iterations);
    }

    public function benchPhpPropertyRead(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $sum += $this->php->propA;
        }
    }
}

final class PhpPropertyHolder
{
    public int $propA = 42;
}
