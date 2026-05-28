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
 * Measures variadic (`...`) parameter calls (issue #2025). The dominant cost
 * is collecting the trailing arguments into an array (zephir_get_args_from).
 *
 * Subjects compare, at a fixed call count:
 *   - the `...` operator (narrow 5-arg and wide 15-arg tails),
 *   - the func_get_args() idiom the `...` operator replaces,
 *   - a pure-PHP variadic baseline,
 * so the report shows both the Zephir-over-PHP margin and the `...`-vs-old-idiom
 * delta, plus how collection cost scales with the argument count.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(1000)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class VariadicBench
{
    private Workload $stub;
    private PhpVariadicHolder $php;
    private int $iterations = 100;

    public function setUp(): void
    {
        $this->stub = new Workload();
        $this->php = new PhpVariadicHolder();
    }

    public function benchZephirVariadicNarrow(): void
    {
        $this->stub->variadicSumNarrowLoop($this->iterations);
    }

    public function benchZephirVariadicWide(): void
    {
        $this->stub->variadicSumWideLoop($this->iterations);
    }

    public function benchZephirFuncGetArgs(): void
    {
        $this->stub->funcGetArgsSumLoop($this->iterations);
    }

    public function benchPhpVariadic(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $sum += $this->php->variadicSum(1, 2, 3, 4, 5);
        }
    }
}

final class PhpVariadicHolder
{
    public function variadicSum(...$numbers): int
    {
        $sum = 0;
        foreach ($numbers as $v) {
            $sum += (int) $v;
        }

        return $sum;
    }
}
