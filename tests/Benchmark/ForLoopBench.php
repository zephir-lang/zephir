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

    /**
     * Zephir `for...in` over an Iterator with `continue` (code path repaired
     * by issue #2546). The first-iteration guard (zend_bool flag)
     * adds one branch per iteration; this shows its overhead versus
     * benchZephirForOverIterator and the PHP baseline below.
     */
    public function benchZephirForOverIteratorWithContinue(): void
    {
        $this->stub->forOverIteratorWithContinue();
    }

    public function benchPhpForOverIteratorWithContinue(): void
    {
        $sum = 0;
        $iter = new ArrayIterator([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
        foreach ($iter as $v) {
            if ($v % 2 === 0) {
                continue;
            }
            $sum += (int) $v;
        }
    }

    /**
     * `for i in 0..n` over the `..` range operator (#2433), now an integer
     * counting loop. Paired with the explicit range() form and the pure-PHP
     * baseline: all three should be in the same ballpark, and far ahead of the
     * old array-materialising path the operator used to take.
     */
    public function benchZephirForOverRangeOperator(): void
    {
        $this->stub->sumRangeOperator(1000);
    }

    public function benchZephirForOverRangeFunction(): void
    {
        $this->stub->sumRangeFn(1000);
    }

    public function benchPhpForOverRange(): void
    {
        $total = 0;
        for ($i = 0; $i <= 1000; $i++) {
            $total += $i;
        }
    }
}
