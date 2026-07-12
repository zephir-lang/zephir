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
 * Measures building an (n+1) x (n+1) matrix via `let output[i][j] = 1` in
 * nested loops (issue #1884). Paired with a pure-PHP nested-`for` baseline so
 * the report shows the Zephir-vs-PHP margin for multi-dimensional writes.
 *
 * The Zephir subject exercises the final-offset fast-path in
 * `zephir_array_update_multi_ex`: the pure-PHP row is the canary — it must
 * stay flat across the fix while the Zephir row improves.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(1000)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class MultiDimArrayBench
{
    private const MATRIX_N = 30;

    private Workload $stub;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchZephirBuildMatrix(): void
    {
        $this->stub->buildMatrix(self::MATRIX_N);
    }

    public function benchPhpBuildMatrix(): void
    {
        $output = [];
        for ($i = 0; $i <= self::MATRIX_N; $i++) {
            $output[$i] = [];
            for ($j = 0; $j <= self::MATRIX_N; $j++) {
                $output[$i][$j] = 1;
            }
        }
    }
}
