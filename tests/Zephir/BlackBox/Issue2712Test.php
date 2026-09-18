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
 * `is_private_property()` was a Zephir builtin backed by an optimizer that
 * emitted `zephir_is_private_prop()`, a kernel function no commit ever defined.
 * Optimizer lookup is filename convention only, so any .zep calling the builtin
 * reached that optimizer and produced C that did not compile.
 *
 * The optimizer is gone. What has to stay gone is the emission: the call must
 * now leave the compiler as an ordinary runtime function call, reported as a
 * function that does not exist rather than silently swallowed.
 *
 * Generate-only on purpose. The runtime half of the contract, that the call
 * then fails exactly as it does in PHP, is pinned by
 * tests/zept/issue2712_undefined_call_parity.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2712
 */
final class Issue2712Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    public function testRemovedBuiltinIsNotReportedAsMissingOptimizer(): void
    {
        // A function that does not exist is already reported as
        // nonexistent-function; reporting it a second time as an optimization
        // opportunity would be advice to write an optimizer for nothing.
        $output = $this->generate('-wmissing-optimizer');

        $this->assertStringNotContainsString('missing-optimizer', $output);
    }

    public function testRemovedBuiltinIsReportedAsNonexistentFunction(): void
    {
        $output = $this->generate();

        $this->assertStringContainsString('nonexistent-function', $output);
        $this->assertStringContainsString('is_private_property', $output);
    }

    public function testRemovedBuiltinEmitsNoUndefinedKernelSymbol(): void
    {
        $this->generate();

        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');

        $this->assertStringNotContainsString(
            'zephir_is_private_prop',
            $generated,
            'The builtin emitted a kernel function that has never existed, so the C did not compile.'
        );
        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_FUNCTION("is_private_property"',
            $generated,
            'With no optimizer, the call has to fall through to a runtime call by name.'
        );
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2712-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
        file_put_contents($this->projectDir . '/stub/probe.zep', <<<'ZEP'
namespace Stub;

class Probe
{
    public function check(var name)
    {
        return is_private_property(name);
    }
}
ZEP);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    private function generate(string $extraArgs = ''): string
    {
        $result = $this->runZephir(trim('generate --no-ansi ' . $extraArgs), $this->projectDir);

        $this->assertSame(0, $result['exitCode'], 'A call to a function that does not exist is not a build error');

        return $result['stdout'] . $result['stderr'];
    }
}
