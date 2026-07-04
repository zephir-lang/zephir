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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Bench;

/**
 * Deterministic performance guarantees of generator methods (issue #1849):
 * a generator streams elements in O(1) memory while the equivalent array
 * producer materializes O(n). Memory assertions are stable across runners
 * (unlike timings, which live in tests/Benchmark/GeneratorBench.php).
 *
 * memory_get_usage() deltas around the measured operation (after a GC
 * cycle) are process-shared-safe, so no process isolation is needed —
 * which also matters because the stub extension cannot be loaded by a
 * bare child process (it requires ext-psr and ext-mysqli from the CLI ini).
 */
final class GeneratorMemoryTest extends TestCase
{
    protected function setUp(): void
    {
        gc_collect_cycles();
    }

    private const SMALL = 20000;
    private const LARGE = 200000;

    public function testGeneratorConsumptionIsConstantMemory(): void
    {
        $stub = new Bench();

        $smallGrowth = $this->memoryGrowthOfConsuming($stub->generatorRange(self::SMALL));
        $largeGrowth = $this->memoryGrowthOfConsuming($stub->generatorRange(self::LARGE));

        $this->assertLessThan(
            256 * 1024,
            $largeGrowth,
            'consuming a 200k-element generator must not materialize the sequence'
        );
        $this->assertLessThan(
            64 * 1024,
            abs($largeGrowth - $smallGrowth),
            'generator memory footprint must not scale with the element count'
        );
    }

    public function testArrayProducerMaterializesLinearMemory(): void
    {
        $stub = new Bench();

        $before = memory_get_usage();
        $array  = $stub->arrayRange(self::LARGE);
        $growth = memory_get_usage() - $before;

        // 200k packed longs: >= 1.5 MB on every PHP 8.x layout. The exact
        // number varies by version; the point is the contrast with the
        // generator's flat footprint above.
        $this->assertGreaterThan(1536 * 1024, $growth);
        $this->assertCount(self::LARGE, $array);
    }

    public function testGeneratorYieldsSameSequenceAsArray(): void
    {
        $stub = new Bench();

        $this->assertSame(
            $stub->arrayRange(1000),
            iterator_to_array($stub->generatorRange(1000), false),
            'lazy and eager producers must emit identical sequences'
        );
        $this->assertSame(
            $stub->sumViaArray(1000),
            $stub->sumViaGenerator(1000),
            'extension-internal consumption must agree between generator and array'
        );
    }

    /**
     * @param \Iterator $generator
     */
    private function memoryGrowthOfConsuming($generator): int
    {
        $before = memory_get_usage();
        $last   = null;
        foreach ($generator as $v) {
            $last = $v;
        }
        $growth = memory_get_usage() - $before;

        $this->assertNotNull($last);

        return $growth;
    }
}
