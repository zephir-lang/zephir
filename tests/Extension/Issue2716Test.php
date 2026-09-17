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
use Stub\Issue2716;

/**
 * Converting a parameter opens a Zephir memory frame, which only the end of the
 * method body closes. A body with nothing in it, and a body believed to have no
 * reachable end, never emitted that close, so every call retained the frame,
 * the method globals and the converted parameter: 288 bytes for each
 * `new Phalcon\Annotations\Adapter\Memory()` in the report.
 *
 * PHP retains nothing for the equivalent code, which is what the oracle test at
 * the bottom pins.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2716
 */
final class Issue2716Test extends TestCase
{
    private const SETTLE = 100;
    private const SMALL  = 1000;
    private const LARGE  = 100000;

    private Issue2716 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2716();
        gc_collect_cycles();
    }

    /**
     * The positive control: without it a flat probe reading proves nothing,
     * because it would also be flat if the probe could not measure.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->assertGreaterThan(
            0,
            $this->subject->leakProbeControl(2000),
            'The leak probe cannot detect retention, so its flat readings are meaningless.'
        );
    }

    /**
     * @return array<string, array{0: string, 1: int}>
     */
    public static function probeProvider(): array
    {
        return [
            'empty body, array parameter with a default' => ['constructProbe', self::LARGE],
            'empty body, string parameter'               => ['stringProbe', self::LARGE],
            'empty body, array parameter'                => ['arrayProbe', self::LARGE],
            'comment-only body, string default'          => ['commentProbe', self::LARGE],
            // Each iteration builds and clears an exception, so this one is
            // paced down; a leak still scales with the count either way.
            'body is a try that swallows its throw'      => ['throwProbe', 20000],
        ];
    }

    /**
     * A leak scales with the iteration count, so compare two runs a long way
     * apart rather than pinning an absolute figure: the first call to
     * memory_get_usage() itself costs a call-cache slot, and that one-off is not
     * what this is looking for.
     *
     * @dataProvider probeProvider
     * @depends      testLeakProbeCanMeasureGrowth
     */
    public function testCallingTheMethodRetainsNothing(string $probe, int $large): void
    {
        // Settle interned strings and call caches.
        $this->subject->$probe(self::SETTLE);

        $this->assertSame(
            $this->subject->$probe(self::SMALL),
            $this->subject->$probe($large),
            'Retained memory grows with the iteration count, so the memory frame is never released.'
        );
    }

    /**
     * PHP is the oracle. The same class written in plain PHP retains nothing per
     * instance, so "flat" is the behaviour being matched, not a threshold picked
     * to make the test pass.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testTheExtensionRetainsAsLittlePerInstanceAsPhpDoes(): void
    {
        $php = new class () {
            public function __construct(array $options = [])
            {
            }
        };

        $this->assertSame(
            $this->bytesPerInstance(static fn (): object => new $php()),
            $this->bytesPerInstance(static fn (): object => new Issue2716()),
            'A Zephir instance must retain no more than the equivalent PHP instance does.'
        );
    }

    private function bytesPerInstance(callable $make): int
    {
        // Warm up the class entry and the call cache before measuring.
        $make();
        $make();
        gc_collect_cycles();

        $iterations = 5000;
        $before     = memory_get_usage();

        for ($i = 0; $i < $iterations; ++$i) {
            $make();
        }

        return intdiv(memory_get_usage() - $before, $iterations);
    }
}
