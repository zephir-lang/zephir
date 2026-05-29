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
 * Measures `this->method()` dispatch in a hot loop.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class MethodCallBench
{
    private Workload $stub;
    private PhpMethodHolder $php;
    private int $iterations = 100;

    public function setUp(): void
    {
        $this->stub = new Workload();
        $this->php = new PhpMethodHolder();
    }

    public function benchZephirMethodDispatch(): void
    {
        $this->stub->methodDispatchLoop($this->iterations);
    }

    public function benchPhpMethodDispatch(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $sum += $this->php->returnConst();
        }
    }
}

final class PhpMethodHolder
{
    public function returnConst(): int
    {
        return 7;
    }
}
