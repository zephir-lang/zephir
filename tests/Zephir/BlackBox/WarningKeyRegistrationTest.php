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
 * A warning only prints when its key is registered, and six keys the compiler
 * emits never were, so six diagnostics were raised and thrown away. Nothing
 * reported it, because "key not registered" and "warning turned off" read back
 * as the same falsy value.
 *
 * These build a real project and read what the compiler actually says.
 *
 * Most of the keys sit behind `hasDifferentDynamicType()`, which only fires
 * for a variable whose dynamic type is known and wrong. Static type inference
 * narrows the simple literal cases to a C type first and the compiler then
 * raises a hard error instead, so these turn inference off to reach the
 * dynamic path the warnings were written for.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2727
 */
final class WarningKeyRegistrationTest extends TestCase
{
    use RunsZephirCommands;

    private const DYNAMIC = '-fno-static-type-inference -fno-local-context-pass';

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-warnings-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);

        $this->writeConfig(['namespace' => 'stub', 'name' => 'stub']);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @return array<string, array{0: string, 1: string, 2: string}>
     */
    public function unregisteredWarningProvider(): array
    {
        return [
            'non-valid-unset' => [
                'non-valid-unset',
                'unset b[0];',
                'Possible attempt to use non array/object in unset operator',
            ],
            'non-valid-require' => [
                'non-valid-require',
                'require b;',
                'Possible attempt to use invalid type as path in "require" operator',
            ],
            'non-valid-require-once' => [
                'non-valid-require-once',
                'require_once b;',
                'Possible attempt to use invalid type as path in "require_once" operator',
            ],
        ];
    }

    /**
     * @dataProvider unregisteredWarningProvider
     */
    public function testShouldPrintAWarningThatCouldNeverPrintBefore(
        string $key,
        string $statement,
        string $message
    ): void {
        $this->writeZep('probe.zep', $this->classWith($statement));

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringContainsString($message, $output);
        $this->assertStringContainsString("[$key]", $output);
    }

    /**
     * @dataProvider unregisteredWarningProvider
     */
    public function testShouldSilenceThatWarningOnDemand(string $key, string $statement): void
    {
        $this->writeZep('probe.zep', $this->classWith($statement));

        $output = $this->generate(self::DYNAMIC . ' -W' . $key);

        $this->assertStringNotContainsString("[$key]", $output);
    }

    /**
     * `extra-parentheses` needs no help from the optimizer flags.
     */
    public function testShouldPrintExtraParentheses(): void
    {
        $this->writeZep('probe.zep', $this->classWith('return ((b + 2));'));

        $this->assertStringContainsString('[extra-parentheses]', $this->generate());
        $this->assertStringNotContainsString(
            '[extra-parentheses]',
            $this->generate('-Wextra-parentheses')
        );
    }

    /**
     * `invalid-array-offset` was emitted and never registered while
     * `invalid-array-index` was registered, documented and emitted nowhere.
     * They are the same diagnostic.
     */
    public function testShouldPrintTheArrayIndexWarningUnderItsRegisteredKey(): void
    {
        $this->writeZep('probe.zep', <<<'ZEP'
namespace Stub;

class Probe
{
    public function run()
    {
        var a, c = 1.2;

        let a = new \stdClass();
        let a->b[c] = 1;
    }
}
ZEP);

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringContainsString(
            'Possible attempt to use non string/long dynamic variable as array index',
            $output
        );
        $this->assertStringContainsString('[invalid-array-index]', $output);
        $this->assertStringNotContainsString('invalid-array-offset', $output);
    }

    /**
     * An integer assigned to a `var` is recorded as the dynamic type `long`,
     * never `int`, and the guard only listed `int`. A declared `int` never
     * reaches the check at all, because the outer test requires the index to
     * be a dynamic variable. So the list could not match an integer and every
     * `let obj->prop[n] = v` with a computed offset was reported.
     *
     * cphalcon hits this once, on `count(this->data) + 1`.
     */
    public function testShouldNotWarnOnAnIntegerArrayIndex(): void
    {
        $this->writeZep('probe.zep', <<<'ZEP'
namespace Stub;

class Probe
{
    protected store = [];

    public function run(array data) -> <static>
    {
        var n;

        let n = count(data) + 1;
        let this->store[n] = 1;

        return this;
    }
}
ZEP);

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringNotContainsString('[invalid-array-index]', $output);
    }

    /**
     * The message used to say "increment" for both, because the decrementing
     * class held the wording and the incrementing one only extends it.
     */
    public function testShouldNameTheRightOperationOnANonObjectProperty(): void
    {
        $this->writeZep('probe.zep', <<<'ZEP'
namespace Stub;

class Probe
{
    public function down()
    {
        var b = 1.2;

        let b->x--;
    }

    public function up()
    {
        var b = 1.2;

        let b->x++;
    }
}
ZEP);

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringContainsString('Possible attempt to decrement non-object dynamic variable', $output);
        $this->assertStringContainsString('Possible attempt to increment non-object dynamic variable', $output);
        $this->assertStringNotContainsString('non-object-update', $output);
    }

    /**
     * The guard was missing the narrowing every sibling site has: it ran the
     * dynamic-type test on a natively typed variable too, and a native
     * `array` carries no dynamic types at all, so the check was true for
     * every `unset arr[key]` on a declared array.
     *
     * Nobody noticed because the key was never registered, so the warning
     * could not print. Registering it surfaced three of these in Zephir's own
     * `stub/` directory.
     */
    public function testShouldNotWarnUnsettingAnOffsetOfADeclaredArray(): void
    {
        $this->writeZep('probe.zep', <<<'ZEP'
namespace Stub;

class Probe
{
    public function run(array data, string! key) -> array
    {
        unset data[key];

        return data;
    }
}
ZEP);

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringNotContainsString('[non-valid-unset]', $output);
    }

    public function testShouldReportAMisspelledWarningKeyInConfigJson(): void
    {
        $this->writeConfig([
            'namespace' => 'stub',
            'name'      => 'stub',
            'warnings'  => ['unusd-variable' => true],
        ]);
        $this->writeZep('probe.zep', $this->classWith('return b;'));

        $output = $this->generate();

        $this->assertStringContainsString(
            '"unusd-variable" is not a recognized warnings key and will be ignored',
            $output
        );
        $this->assertStringContainsString('Did you mean "unused-variable"?', $output);
    }

    public function testShouldReportAMisspelledWarningFlag(): void
    {
        $this->writeZep('probe.zep', $this->classWith('return b;'));

        $this->assertStringContainsString(
            '"unusd-variable" is not a recognized warnings key and will be ignored',
            $this->generate('-Wunusd-variable')
        );
    }

    public function testShouldReportAMisspelledOptimizationFlag(): void
    {
        $this->writeZep('probe.zep', $this->classWith('return b;'));

        $output = $this->generate('-fno-loca-context-pass');

        $this->assertStringContainsString(
            '"loca-context-pass" is not a recognized optimizations key and will be ignored',
            $output
        );
        $this->assertStringContainsString('Did you mean "local-context-pass"?', $output);
    }

    /**
     * A `warnings` block that omits a key used to delete it, so the omitted
     * warning went silent. It keeps its default now.
     */
    public function testShouldKeepDefaultWarningsAlongsideAPartialWarningsBlock(): void
    {
        $this->writeConfig([
            'namespace' => 'stub',
            'name'      => 'stub',
            'warnings'  => ['missing-optimizer' => true],
        ]);
        $this->writeZep('probe.zep', $this->classWith('unset b[0];'));

        $output = $this->generate(self::DYNAMIC);

        $this->assertStringContainsString('[non-valid-unset]', $output);
        $this->assertStringNotContainsString('is not recognized', $output);
    }

    public function testShouldSayNothingAboutAConfigItUnderstands(): void
    {
        $this->writeZep('probe.zep', $this->classWith('return b;'));

        $this->assertStringNotContainsString('is not a recognized', $this->generate());
    }

    private function classWith(string $statement): string
    {
        return <<<ZEP
namespace Stub;

class Probe
{
    public function run()
    {
        var b = 1.2;

        $statement
    }
}
ZEP;
    }

    private function generate(string $extraArgs = ''): string
    {
        $result = $this->runZephir(trim('generate --no-ansi ' . $extraArgs), $this->projectDir);

        $this->assertSame(0, $result['exitCode'], 'A warning must not fail the build');

        return $result['stdout'] . $result['stderr'];
    }

    private function writeConfig(array $config): void
    {
        file_put_contents($this->projectDir . '/config.json', json_encode($config));
    }

    private function writeZep(string $relativePath, string $content): void
    {
        file_put_contents($this->projectDir . '/stub/' . $relativePath, $content);
    }
}
