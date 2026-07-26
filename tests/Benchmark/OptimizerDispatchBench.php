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
 * Quantifies the cost the `missing-optimizer` warning reports (#2468): a call
 * with no applicable optimizer is dispatched by name through the Zend function
 * table instead of being inlined as a kernel call.
 *
 * Each pair calls the *same* PHP function for the *same* result; only the call
 * form differs, so the delta isolates dispatch overhead:
 *
 *   count(a)        -> inlined zephir_fast_count_int
 *   count(a, 0)     -> CountOptimizer declines on arity -> runtime call
 *   implode("", a)  -> inlined zephir_fast_join
 *   implode(a)      -> ImplodeOptimizer declines on arity -> runtime call
 *
 * benchAcos is the before/after probe for the ACos/ASin optimizer rename: on a
 * case-sensitive filesystem the optimizer never loaded, so acos() was a runtime
 * call; once reachable it compiles to a bare libm acos().
 *
 * benchCountPhp is context, not a like-for-like baseline: PHP compiles a
 * single-argument count() on an array to the dedicated ZEND_COUNT opcode, so it
 * never pays for a function call either. Read it as "what the engine does when
 * it also avoids dispatch", not as "the cost of a PHP function call".
 *
 * Run baseline / comparison:
 *   php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
 *       tests/Benchmark/OptimizerDispatchBench.php --report=aggregate \
 *       --tag=baseline
 *   # ...apply change, rebuild...
 *   php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
 *       tests/Benchmark/OptimizerDispatchBench.php --report=aggregate \
 *       --ref=baseline
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(10)]
#[Bench\Revs(1000)]
#[Bench\Warmup(3)]
#[Bench\OutputTimeUnit('microseconds')]
final class OptimizerDispatchBench
{
    private array $items = [];
    private int $iterations = 5000;
    private array $strings = [];
    private Workload $stub;

    public function setUp(): void
    {
        $this->stub    = new Workload();
        $this->items   = range(1, 10);
        $this->strings = ['a', 'bb', 'ccc', 'dddd', 'ee', 'f', 'gg', 'hhh', 'i', 'jj'];
    }

    public function benchAcos(): void
    {
        $this->stub->acosLoop($this->iterations);
    }

    public function benchCountOptimized(): void
    {
        $this->stub->countOptimizedLoop($this->items, $this->iterations);
    }

    public function benchCountPhp(): void
    {
        $sum = 0;
        for ($i = 0; $i < $this->iterations; ++$i) {
            $sum += count($this->items);
        }
    }

    public function benchCountUnoptimized(): void
    {
        $this->stub->countUnoptimizedLoop($this->items, $this->iterations);
    }

    public function benchImplodeOptimized(): void
    {
        $this->stub->implodeOptimizedLoop($this->strings, $this->iterations);
    }

    public function benchImplodeUnoptimized(): void
    {
        $this->stub->implodeUnoptimizedLoop($this->strings, $this->iterations);
    }
}
