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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;
use Zephir\Zept\ZeptParser;
use Zephir\Zept\ZeptResult;
use Zephir\Zept\ZeptRunner;

use function array_values;
use function getenv;
use function glob;
use function preg_match;
use function sort;

/**
 * End-to-end bridge: discovers every `tests/zept/*.zept`, compiles it through
 * the (PHP-parser-forced) Zephir toolchain, runs its --USAGE-- against the
 * freshly built extension, and asserts the output matches.
 *
 * This is the thin PHPUnit adapter over the dependency-free {@see ZeptRunner};
 * it lives in the BlackBox suite because each case performs a real build.
 *
 * Each case is a full cold `phpize`/`configure`/`make`, so the suite is the
 * slowest part of CI. It is tagged `@group zept` so the rest of the BlackBox
 * suite can run on every matrix leg via `--exclude-group zept`, while this
 * group runs only where it is worth the cost (see `.ci/run-zept-parallel.sh`),
 * split across parallel shards via the `ZEPT_SHARD` env (see {@see zeptFiles}).
 *
 * @group zept
 */
final class ZeptSuiteTest extends TestCase
{
    /**
     * @dataProvider zeptFiles
     */
    public function testZeptCasePasses(string $path): void
    {
        $zept   = (new ZeptParser())->parse((string) file_get_contents($path), $path);
        $runner = new ZeptRunner(\dirname(__DIR__, 3) . '/zephir');
        $result = $runner->run($zept);

        if ($result->status === ZeptResult::SKIP) {
            $this->markTestSkipped($result->reason);
        }

        $this->assertSame(
            ZeptResult::PASS,
            $result->status,
            sprintf(
                "%s: %s\n--- expected ---\n%s\n--- actual ---\n%s",
                basename($path),
                $result->reason,
                $result->expected,
                $result->actual
            )
        );
    }

    /**
     * Yields the `.zept` cases, optionally narrowed to one shard so the suite
     * can be run by several PHPUnit processes in parallel. `ZEPT_SHARD="n/total"`
     * (1-based `n`) selects every `total`-th case starting at `n`; the union of
     * all shards is the full set with no overlap. Absent/invalid env runs all.
     *
     * @return iterable<string, array{0: string}>
     */
    public static function zeptFiles(): iterable
    {
        $dir   = \dirname(__DIR__, 3) . '/tests/zept';
        $files = glob($dir . '/*.zept') ?: [];
        sort($files); // stable order so the shard split is deterministic

        [$shard, $total] = self::shard();

        foreach (array_values($files) as $index => $path) {
            if ($total > 1 && $index % $total !== $shard) {
                continue;
            }

            yield basename($path) => [$path];
        }
    }

    /**
     * Parses `ZEPT_SHARD="<n>/<total>"` (1-based `n`) into a zero-based
     * `[shard, total]` pair. Defaults to `[0, 1]` (a single shard: run all).
     *
     * @return array{0: int, 1: int}
     */
    private static function shard(): array
    {
        $spec = (string) getenv('ZEPT_SHARD');
        if (preg_match('#^(\d+)/(\d+)$#', $spec, $m) !== 1) {
            return [0, 1];
        }

        $n     = (int) $m[1];
        $total = (int) $m[2];
        if ($total < 1 || $n < 1 || $n > $total) {
            return [0, 1];
        }

        return [$n - 1, $total];
    }
}
