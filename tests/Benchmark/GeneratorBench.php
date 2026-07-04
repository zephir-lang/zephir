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
 * Generator (`yield`, issue #1849) versus comparable structures.
 *
 * Three comparison axes, each with a pure-PHP twin so the report shows both
 * the Zephir-vs-PHP margin and the generator-vs-eager margin:
 *
 *  1. Consume from PHP userland: Zephir generator vs Zephir array producer
 *     vs native PHP generator vs PHP array vs ArrayIterator. One suspension
 *     + one internal-method resume per element is the expected generator tax.
 *  2. Create only: generators are lazy (O(1) creation), array producers pay
 *     the full materialization up front.
 *  3. Produce+consume entirely inside the extension (`sumVia*`): isolates
 *     the raw C-level suspend/resume cost from PHP calling overhead.
 *
 * Expected asymmetry in axis 3: Zephir's `for x in <object>` dispatches
 * rewind/valid/current/next through ZEPHIR_CALL_METHOD (full
 * zend_call_function per element), while PHP's foreach drives the
 * generator's native get_iterator handlers directly — so consuming a Zephir
 * generator from PHP is currently faster than consuming it from Zephir.
 * Routing Zephir's object for-in through the native handler is a separate
 * optimization opportunity.
 *
 * The *Large subjects (5k elements) also stress sustained suspension; the
 * O(1)-memory guarantee itself is asserted deterministically in
 * tests/Extension/GeneratorMemoryTest.php.
 */
#[Bench\BeforeMethods('setUp')]
#[Bench\Iterations(5)]
#[Bench\Revs(1000)]
#[Bench\Warmup(2)]
#[Bench\OutputTimeUnit('microseconds')]
final class GeneratorBench
{
    private const SMALL = 50;
    private const LARGE = 5000;

    private Workload $stub;

    /**
     * @var array<int, int>
     */
    private array $array;

    public function setUp(): void
    {
        $this->stub  = new Workload();
        $this->array = range(1, self::SMALL);
    }

    /* ---- 1. consume from PHP (50 elements) ----------------------------- */

    public function benchZephirGeneratorConsume(): void
    {
        $sum = 0;
        foreach ($this->stub->generatorRange(self::SMALL) as $v) {
            $sum += $v;
        }
    }

    public function benchZephirArrayConsume(): void
    {
        $sum = 0;
        foreach ($this->stub->arrayRange(self::SMALL) as $v) {
            $sum += $v;
        }
    }

    public function benchZephirGeneratorRelayConsume(): void
    {
        $sum = 0;
        foreach ($this->stub->generatorOverArray($this->array) as $v) {
            $sum += $v;
        }
    }

    public function benchPhpGeneratorConsume(): void
    {
        $gen = static function (int $n) {
            for ($i = 1; $i <= $n; $i++) {
                yield $i;
            }
        };
        $sum = 0;
        foreach ($gen(self::SMALL) as $v) {
            $sum += $v;
        }
    }

    public function benchPhpArrayConsume(): void
    {
        $build = static function (int $n): array {
            $result = [];
            for ($i = 1; $i <= $n; $i++) {
                $result[] = $i;
            }

            return $result;
        };
        $sum = 0;
        foreach ($build(self::SMALL) as $v) {
            $sum += $v;
        }
    }

    public function benchPhpArrayIteratorConsume(): void
    {
        $sum = 0;
        foreach (new ArrayIterator($this->array) as $v) {
            $sum += $v;
        }
    }

    /* ---- 2. create only (laziness: O(1) vs O(n)) ------------------------ */

    public function benchZephirGeneratorCreate(): void
    {
        $this->stub->generatorRange(self::LARGE);
    }

    public function benchZephirArrayCreate(): void
    {
        $this->stub->arrayRange(self::LARGE);
    }

    public function benchPhpGeneratorCreate(): void
    {
        $gen = static function (int $n) {
            for ($i = 1; $i <= $n; $i++) {
                yield $i;
            }
        };
        $gen(self::LARGE);
    }

    /* ---- 3. produce + consume inside the extension ---------------------- */

    public function benchZephirSumViaGenerator(): void
    {
        $this->stub->sumViaGenerator(self::SMALL);
    }

    public function benchZephirSumViaArray(): void
    {
        $this->stub->sumViaArray(self::SMALL);
    }

    public function benchPhpSumViaGenerator(): void
    {
        $gen = static function (int $n) {
            for ($i = 1; $i <= $n; $i++) {
                yield $i;
            }
        };
        $sum = 0;
        foreach ($gen(self::SMALL) as $v) {
            $sum += (int) $v;
        }
    }

    /* ---- large runs: time + mem_peak divergence (5k elements) ----------- */

    #[Bench\Revs(50)]
    public function benchZephirGeneratorConsumeLarge(): void
    {
        $sum = 0;
        foreach ($this->stub->generatorRange(self::LARGE) as $v) {
            $sum += $v;
        }
    }

    #[Bench\Revs(50)]
    public function benchZephirArrayConsumeLarge(): void
    {
        $sum = 0;
        foreach ($this->stub->arrayRange(self::LARGE) as $v) {
            $sum += $v;
        }
    }

    #[Bench\Revs(50)]
    public function benchPhpGeneratorConsumeLarge(): void
    {
        $gen = static function (int $n) {
            for ($i = 1; $i <= $n; $i++) {
                yield $i;
            }
        };
        $sum = 0;
        foreach ($gen(self::LARGE) as $v) {
            $sum += $v;
        }
    }

    #[Bench\Revs(50)]
    public function benchPhpArrayConsumeLarge(): void
    {
        $build = static function (int $n): array {
            $result = [];
            for ($i = 1; $i <= $n; $i++) {
                $result[] = $i;
            }

            return $result;
        };
        $sum = 0;
        foreach ($build(self::LARGE) as $v) {
            $sum += $v;
        }
    }
}
