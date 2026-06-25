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

use function glob;

/**
 * End-to-end bridge: discovers every `tests/zept/*.zept`, compiles it through
 * the (PHP-parser-forced) Zephir toolchain, runs its --USAGE-- against the
 * freshly built extension, and asserts the output matches.
 *
 * This is the thin PHPUnit adapter over the dependency-free {@see ZeptRunner};
 * it lives in the BlackBox suite because each case performs a real build.
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
     * @return iterable<string, array{0: string}>
     */
    public static function zeptFiles(): iterable
    {
        $dir = \dirname(__DIR__, 3) . '/tests/zept';
        foreach (glob($dir . '/*.zept') ?: [] as $path) {
            yield basename($path) => [$path];
        }
    }
}
