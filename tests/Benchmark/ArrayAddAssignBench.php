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
 * Measures the `+=` (add-assign) operator on arrays — PHP array union — added
 * in issue #1280. Two code paths are exercised, each paired with the identical
 * pure-PHP `$a += ...` so the headline metric is the Zephir-over-PHP margin:
 *
 *  - UnionLiteral: `let a += [ ... ]` (compiler path, in-place add_function).
 *  - UnionVar:     untyped `let a += b` (runtime ZEPHIR_ADD_ASSIGN macro path).
 *  - Accumulate:   untyped `let a += b` growing the accumulator (confirms the
 *                  macro merges in place, staying O(n) amortized).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1280
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class ArrayAddAssignBench
{
    private const COUNT = 100;

    private Workload $stub;

    public function setUp(): void
    {
        $this->stub = new Workload();
    }

    public function benchZephirUnionLiteral(): void
    {
        $this->stub->addAssignUnionLiteral(self::COUNT);
    }

    public function benchPhpUnionLiteral(): void
    {
        for ($i = 1; $i <= self::COUNT; $i++) {
            $a = [1, 2, 3, 4, 5];
            $a += [6, 7, 8, 'k1' => 1, 'k2' => 2];
        }
    }

    public function benchZephirUnionVar(): void
    {
        $this->stub->addAssignUnionVar(self::COUNT);
    }

    public function benchPhpUnionVar(): void
    {
        $b = [6, 7, 8, 'k1' => 1, 'k2' => 2];
        for ($i = 1; $i <= self::COUNT; $i++) {
            $a = [1, 2, 3, 4, 5];
            $a += $b;
        }
    }

    public function benchZephirAccumulate(): void
    {
        $this->stub->addAssignAccumulate(self::COUNT);
    }

    public function benchPhpAccumulate(): void
    {
        $a = [];
        for ($i = 1; $i <= self::COUNT; $i++) {
            $b = [];
            $b[$i] = $i;
            $a += $b;
        }
    }
}
