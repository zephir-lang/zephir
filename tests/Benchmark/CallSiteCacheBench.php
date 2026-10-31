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
use Stub\Bench\SiteChild;

/**
 * Calls whose site sits outside any loop, the case the per-site inline cache
 * of #2790 is for. Each Zephir loop iteration makes one such call through a
 * helper method.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(10)]
#[Bench\Revs(200)]
#[Bench\Warmup(3)]
#[Bench\OutputTimeUnit('microseconds')]
final class CallSiteCacheBench
{
    private Workload $stub;
    private SiteChild $child;
    private ArrayIterator $iterator;
    private int $iterations = 2000;

    public function setUp(): void
    {
        $this->stub     = new Workload();
        $this->child    = new SiteChild();
        $this->iterator = new ArrayIterator([1, 2, 3]);
    }

    public function benchZephirThisSite(): void
    {
        $this->stub->siteMethodLoop($this->iterations);
    }

    public function benchZephirOtherSite(): void
    {
        $this->stub->siteOtherLoop($this->child, $this->iterations);
    }

    public function benchZephirSelfSite(): void
    {
        $this->stub->siteStaticLoop($this->iterations);
    }

    public function benchZephirParentSite(): void
    {
        $this->child->siteParentLoop($this->iterations);
    }

    public function benchZephirIteratorSite(): void
    {
        $this->stub->siteIteratorLoop($this->iterator, $this->iterations);
    }

    public function benchPhpThisSite(): void
    {
        $holder = new PhpSiteHolder();
        $holder->siteMethodLoop($this->iterations);
    }
}

final class PhpSiteHolder
{
    public function siteMethodLoop(int $n): int
    {
        $sum = 0;
        for ($i = 0; $i < $n; $i++) {
            $sum += $this->siteMethodOnce();
        }

        return $sum;
    }

    public function siteMethodOnce(): int
    {
        return $this->returnConst();
    }

    public function returnConst(): int
    {
        return 7;
    }
}
