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
 * Isolates the per-call dispatch cost of Zephir-compiled calls for the
 * FastCall investigation (#1510). Each subject loops a trivial callee
 * 5000 times inside the extension, so the reported time is dominated by the
 * call machinery (cache lookup + fcic setup + zend_call_function) rather than
 * the callee body or PHPBench's own per-rev overhead.
 *
 * Run baseline / comparison:
 *   php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
 *       tests/Benchmark/CallDispatchBench.php --report=aggregate \
 *       --tag=baseline
 *   # ...apply kernel change, rebuild...
 *   php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
 *       tests/Benchmark/CallDispatchBench.php --report=aggregate \
 *       --ref=baseline
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(10)]
#[Bench\Revs(2000)]
#[Bench\Warmup(3)]
#[Bench\OutputTimeUnit('microseconds')]
final class CallDispatchBench
{
    private Workload $stub;
    private int $iterations = 5000;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchInstanceDispatch(): void
    {
        $this->stub->methodDispatchLoop($this->iterations);
    }

    public function benchFinalDispatch(): void
    {
        $this->stub->finalDispatchLoop($this->iterations);
    }

    public function benchStaticDispatch(): void
    {
        $this->stub->staticDispatchLoop($this->iterations);
    }
}
