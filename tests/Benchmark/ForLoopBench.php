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

use ArrayIterator;
use PhpBench\Attributes as Bench;
use Stub\Bench as Workload;

/**
 * Measures `for ... in expr` over array and Iterator. Paired with pure-PHP
 * `foreach` baselines so the report shows the Zephir speedup margin.
 *
 * The Iterator subjects are sensitive to the dead-branch elimination from
 * issue #1878 (skip IS_ARRAY wrapper when iterand type is known).
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(1000)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class ForLoopBench
{
    private Workload $stub;

    /**
     * @var array<int, int>
     */
    private array $array;

    public function setUp(): void
    {
        $this->stub = new Workload();
        $this->array = range(1, 50);
    }

    public function benchZephirForOverArray(): void
    {
        $this->stub->forOverArray($this->array);
    }

    public function benchPhpForOverArray(): void
    {
        $sum = 0;
        foreach ($this->array as $v) {
            $sum += (int) $v;
        }
    }

    public function benchZephirForOverIterator(): void
    {
        $this->stub->forOverIterator();
    }

    public function benchPhpForOverIterator(): void
    {
        $sum = 0;
        $iter = new ArrayIterator([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
        foreach ($iter as $v) {
            $sum += (int) $v;
        }
    }
}
