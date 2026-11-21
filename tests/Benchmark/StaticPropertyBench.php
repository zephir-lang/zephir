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
use Stub\Bench\Foo;

/**
 * Measures `self::x` reads, writes, isset and empty in a hot loop, each
 * answered from the access's cache record after the first (#2805), against
 * the same loops in PHP.
 */
#[Bench\Iterations(5)]
#[Bench\Revs(500)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class StaticPropertyBench
{
    private static int $a = 0;

    private int $iterations = 100;

    public function benchZephirRead(): void
    {
        Foo::readStatic($this->iterations);
    }

    public function benchZephirWrite(): void
    {
        Foo::writeStatic($this->iterations);
    }

    public function benchZephirIsset(): void
    {
        Foo::issetStatic($this->iterations);
    }

    public function benchZephirEmpty(): void
    {
        Foo::emptyStatic($this->iterations);
    }

    public function benchPhpRead(): void
    {
        for ($i = 1; $i <= $this->iterations; $i++) {
            $x = self::$a;
        }
    }

    public function benchPhpWrite(): void
    {
        for ($i = 1; $i <= $this->iterations; $i++) {
            self::$a = 0;
        }
    }
}
