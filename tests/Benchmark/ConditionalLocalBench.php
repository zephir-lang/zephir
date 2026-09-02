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
 * Prices the fix for #2679. A local assigned only inside a conditional is
 * registered with the memory frame at its declaration, which costs one extra
 * zephir_memory_observe() per call of the method that declares it. No method
 * gains a memory frame from it: the conditional assignment already needed one.
 *
 * Compare across compilers, not across subjects:
 *
 *   php ... vendor/bin/phpbench run --report=aggregate --tag=base   # before
 *   php ... vendor/bin/phpbench run --report=aggregate --ref=base   # after
 *
 * The percentage is on the time metric, so negative is faster. Two of the three
 * subjects are controls that cannot change: `definiteLocal` is compiled to the
 * same C either way, and the PHP subject never touches the compiler. Read the
 * affected subject's delta against theirs, not against zero. The inner loop is
 * deliberately long so the per-call cost is not lost in PHPBench's own
 * per-revolution overhead.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(10)]
#[Bench\Revs(50)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class ConditionalLocalBench
{
    private Workload $stub;
    private int $iterations = 20000;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchZephirConditionalLocal(): void
    {
        $this->stub->conditionalLocalLoop($this->iterations);
    }

    public function benchZephirDefiniteLocal(): void
    {
        $this->stub->definiteLocalLoop($this->iterations);
    }

    public function benchPhpConditionalLocal(): void
    {
        $hits = 0;
        for ($i = 0; $i < $this->iterations; $i++) {
            $hits += $this->phpStep(($i % 2) === 0);
        }
    }

    private function phpStep(bool $flag): int
    {
        if ($flag) {
            $x = 'set';
        }

        return is_string($x ?? null) ? 1 : 0;
    }
}
