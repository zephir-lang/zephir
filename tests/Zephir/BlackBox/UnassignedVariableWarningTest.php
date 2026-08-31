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
 * A local that is read but never assigned now evaluates to null, the same value
 * PHP gives an unset variable. PHP reports it at runtime; Zephir knows it at
 * compile time and reports it there.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2654
 */
final class UnassignedVariableWarningTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    public function testUnassignedLocalIsReportedByDefault(): void
    {
        $this->writeZep('reported.zep', <<<'ZEP'
namespace Stub;

class Reported
{
    public function run()
    {
        var x;

        return x;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringContainsString('unassigned-variable', $output);
        $this->assertStringContainsString('read but never assigned', $output);
        $this->assertStringContainsString('Stub\Reported::run', $output);
    }

    public function testWarningCanBeDisabled(): void
    {
        $this->writeZep('disabled.zep', <<<'ZEP'
namespace Stub;

class Disabled
{
    public function run()
    {
        var x;

        return x;
    }
}
ZEP);

        $output = $this->generate('-Wunassigned-variable');

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    public function testAssignedLocalIsNotReported(): void
    {
        $this->writeZep('assigned.zep', <<<'ZEP'
namespace Stub;

class Assigned
{
    public function run()
    {
        var x;

        let x = 1;

        return x;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    public function testDeclarationWithAValueIsNotReported(): void
    {
        $this->writeZep('initialized.zep', <<<'ZEP'
namespace Stub;

class Initialized
{
    public function run()
    {
        var x = 1;

        return x;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    /**
     * A parameter is filled by the engine, not by an assignment in the body.
     */
    public function testParameterIsNotReported(): void
    {
        $this->writeZep('parameter.zep', <<<'ZEP'
namespace Stub;

class Parameter
{
    public function run(var a, string b = "", array c = [])
    {
        return [a, b, c];
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    /**
     * A declaration nobody reads is already covered by `unused-variable`, and
     * the compiler does not even emit it, so it must not be reported twice.
     */
    public function testNeverReadLocalIsOnlyReportedAsUnused(): void
    {
        $this->writeZep('neverread.zep', <<<'ZEP'
namespace Stub;

class NeverRead
{
    public function run() -> int
    {
        var x;

        return 1;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringContainsString('unused-variable', $output);
        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    /**
     * An out-parameter is written by the callee, so it is assigned even though
     * no `let` mentions it. zephir_preg_match() fills `matches` in.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2654
     */
    public function testPregMatchOutParameterIsNotReported(): void
    {
        $this->writeZep('pregmatch.zep', <<<'ZEP'
namespace Stub;

class PregMatch
{
    public function run(string subject) -> array
    {
        var matches;

        preg_match("/[a-z]+/", subject, matches);

        return matches;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    /**
     * The same for a builtin whose parameter is by reference, which reaches the
     * generic ZEPHIR_MAKE_REF path rather than a dedicated optimizer.
     */
    public function testByReferenceArgumentIsNotReported(): void
    {
        $this->writeZep('byref.zep', <<<'ZEP'
namespace Stub;

class ByRef
{
    public function run(string subject) -> int
    {
        var count;

        str_replace("a", "b", subject, count);

        return count;
    }
}
ZEP);

        $output = $this->generate();

        $this->assertStringNotContainsString('unassigned-variable', $output);
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-unassigned-' . bin2hex(random_bytes(6));
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
