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

/**
 * A function call with no applicable optimizer compiles to a runtime
 * ZEPHIR_CALL_FUNCTION, dispatched by name through the Zend function table
 * instead of being inlined as a kernel call. `zephir generate` must be able to
 * report those calls so the author can see the optimization opportunities.
 *
 * The report is opt-in (`-wmissing-optimizer`) and deduplicated: one warning per
 * distinct function name per run, anchored at its first call site.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2468
 */
final class MissingOptimizerWarningTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    public function testDeclinedOptimizerIsReported(): void
    {
        // count() has an optimizer, but it only handles the single-argument
        // form; count(arr, 0) silently falls back to a runtime call.
        $this->writeZep('declined.zep', <<<'ZEP'
namespace Stub;

class Declined
{
    public function run(array arr) -> int
    {
        return count(arr, 0);
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringContainsString('missing-optimizer', $output);
        $this->assertStringContainsString('count', $output);
        $this->assertStringContainsString('does not handle this call form', $output);
    }

    public function testNonexistentFunctionIsNotReportedAsMissingOptimizer(): void
    {
        $this->writeZep('missing.zep', <<<'ZEP'
namespace Stub;

class Missing
{
    public function run()
    {
        return this_function_does_not_exist();
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringContainsString('nonexistent-function', $output);
        $this->assertStringNotContainsString('missing-optimizer', $output);
    }

    public function testOptimizedFunctionIsNotReported(): void
    {
        $this->writeZep('optimized.zep', <<<'ZEP'
namespace Stub;

class Optimized
{
    public function run(string str) -> int
    {
        return strlen(str);
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringNotContainsString('missing-optimizer', $output);
    }

    public function testReportIsOffByDefault(): void
    {
        $this->writeZep('defaultoff.zep', <<<'ZEP'
namespace Stub;

class DefaultOff
{
    public function run(array arr)
    {
        return array_product(arr);
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('missing-optimizer', $output);
        $this->assertStringNotContainsString('array_product', $output);
    }

    public function testUnoptimizedFunctionIsReported(): void
    {
        $this->writeZep('unoptimized.zep', <<<'ZEP'
namespace Stub;

class Unoptimized
{
    public function run(array arr)
    {
        return array_product(arr);
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringContainsString('missing-optimizer', $output);
        $this->assertStringContainsString('array_product', $output);
        $this->assertStringContainsString('has no Zephir optimizer', $output);
    }

    public function testUserlandZephirFunctionIsNotReported(): void
    {
        // Declared after the namespace statement so it is namespaced rather
        // than global; a global function gets a `g_` internal name the
        // compiler's own lookup does not probe.
        $this->writeZep('userland.zep', <<<'ZEP'
namespace Stub;

function stubHelper(a)
{
    return a;
}

class Userland
{
    public function run(a)
    {
        return stubHelper(a);
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringNotContainsString('missing-optimizer', $output);
    }

    public function testWarningIsReportedOncePerFunction(): void
    {
        $this->writeZep('repeated.zep', <<<'ZEP'
namespace Stub;

class Repeated
{
    public function one(array arr)
    {
        return array_product(arr);
    }

    public function two(array arr)
    {
        return array_product(arr);
    }

    public function three(array arr)
    {
        return array_product(arr);
    }
}
ZEP);

        $output = $this->generate('-wmissing-optimizer');

        $this->assertSame(
            1,
            substr_count($output, 'missing-optimizer'),
            'The same unoptimized function must be reported once per run, not once per call site.'
        );
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-missopt-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    private function generate(string $extraArgs = ''): string
    {
        $result = $this->runZephir(trim('generate --no-ansi ' . $extraArgs), $this->projectDir);

        $this->assertSame(0, $result['exitCode'], 'A warning must not fail the build');

        return $result['stdout'] . $result['stderr'];
    }

    private function writeZep(string $relativePath, string $content): void
    {
        $full = $this->projectDir . '/stub/' . $relativePath;
        if (!is_dir(dirname($full))) {
            mkdir(dirname($full), 0777, true);
        }
        file_put_contents($full, $content);
    }
}
