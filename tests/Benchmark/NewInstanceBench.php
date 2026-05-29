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
 * Measures `new T()` followed by an immediate method call in a hot loop.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(200)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class NewInstanceBench
{
    private Workload $stub;
    private int $iterations = 50;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchZephirNewInstanceLoop(): void
    {
        $this->stub->newInstanceLoop($this->iterations);
    }

    public function benchPhpNewInstanceLoop(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $obj = new PhpInstanceTarget();
            $sum += $obj->returnConst();
        }
    }
}

final class PhpInstanceTarget
{
    public function returnConst(): int
    {
        return 7;
    }
}
