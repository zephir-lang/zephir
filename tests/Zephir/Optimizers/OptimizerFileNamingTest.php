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

namespace Zephir\Test\Optimizers;

use PHPUnit\Framework\TestCase;
use Zephir\Name;

use function basename;
use function glob;
use function preg_replace;
use function strtolower;

/**
 * Optimizers are resolved purely by filename: FunctionCall::optimize() looks for
 * `Name::camelize($funcName) . 'Optimizer.php'`. The function name it camelizes has
 * already been lowercased, so camelize() can never emit two consecutive capitals —
 * a file named `ACosOptimizer.php` is therefore unreachable on a case-sensitive
 * filesystem, while a case-insensitive one (macOS, Windows) still matches it. That
 * makes a misnamed optimizer a silent, platform-dependent behaviour difference.
 *
 * This test locks the naming convention so such a file cannot be added again.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2468
 */
final class OptimizerFileNamingTest extends TestCase
{
    public static function optimizerFileProvider(): iterable
    {
        $pattern = \constant('ZEPHIRPATH') . 'src/Optimizers/FunctionCall/*Optimizer.php';

        foreach (glob($pattern) ?: [] as $path) {
            $name = basename($path, 'Optimizer.php');

            yield $name => [$name];
        }
    }

    /**
     * @dataProvider optimizerFileProvider
     */
    public function testOptimizerFileNameIsReachableByCamelize(string $name): void
    {
        // Reverse the camelize: split on lower/digit -> upper boundaries.
        $functionName = strtolower((string) preg_replace('/(?<=[a-z0-9])(?=[A-Z])/', '_', $name));

        $this->assertSame(
            $name,
            Name::camelize($functionName),
            "{$name}Optimizer.php can never be found: FunctionCall::optimize() looks up "
            . "'" . Name::camelize($functionName) . "Optimizer.php' for the function "
            . "'{$functionName}'. Rename the file and its class."
        );
    }
}
