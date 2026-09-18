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

use function file_put_contents;
use function ucfirst;

/**
 * Compile-time validation of PHP attributes (issue #2466), in a real project so
 * the whole class registry is populated — which the isolated codegen harness
 * does not do, and which the `#[\Override]` and inherited-member checks need.
 *
 * PHP keeps an attribute argument as a constant AST and evaluates it at
 * reflection time. An extension has to hand the engine a zval at MINIT, so
 * Zephir folds the argument during `generate` and anything it cannot fold is a
 * compile error rather than a deferred one.
 *
 * The pure-PHP parser is forced so the checks do not depend on the attribute
 * grammar being present in the installed C extension.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
final class AttributeErrorsTest extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        putenv('ZEPHIR_FORCE_PHP_PARSER=1');
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
        putenv('ZEPHIR_FORCE_PHP_PARSER');
    }

    /**
     * @param array<string, string> $extraSources basename => body, written
     *                                            alongside sample.zep
     *
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, string $sample, array $extraSources = []): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $namespace = ucfirst($name);
        file_put_contents(
            $projectDir . '/' . $name . '/sample.zep',
            "namespace {$namespace};\n\n" . $sample . "\n"
        );
        foreach ($extraSources as $basename => $body) {
            file_put_contents(
                $projectDir . '/' . $name . '/' . $basename . '.zep',
                "namespace {$namespace};\n\n" . $body . "\n"
            );
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }

    public function testFailsOnANonConstantArgument(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attrnonconst',
            "#[Marker(new Sample())]\nclass Sample\n{\n}"
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('is not a compile-time constant expression', $stderr);
    }

    public function testFailsOnACharArgument(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attrchar',
            "#[Marker('a')]\nclass Sample\n{\n}"
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('char is a byte value', $stderr);
    }

    public function testFailsOnStaticClassArgument(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attrlsb',
            "#[Marker(static::class)]\nclass Sample\n{\n}"
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('late static binding', $stderr);
    }

    /**
     * The grammar accepts the prefix on any class member, and a use-trait
     * statement is one. PHP has no such attribute target, and dropping the
     * attribute silently would be worse than saying so.
     */
    public function testFailsOnAnAttributedUseStatement(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attruse',
            "class Sample\n{\n    #[Marker]\n    use Helper;\n}",
            ['helper' => "trait Helper\n{\n    public function help()\n    {\n        return 1;\n    }\n}"]
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Attributes are not supported on a `use` statement', $stderr);
    }

    public function testFailsWhenOverrideHasNoMatchingParentMethod(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attroverride',
            "class Sample extends Base\n{\n    #[\\Override]\n    public function absent()\n    {\n        return 1;\n    }\n}",
            ['base' => "class Base\n{\n    public function present()\n    {\n        return 1;\n    }\n}"]
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString(
            'has #[\\Override] attribute, but no matching parent method exists',
            $stderr
        );
    }

    public function testOverrideOfAnActualParentMethodCompiles(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attroverrideok',
            "class Sample extends Base\n{\n    #[\\Override]\n    public function present()\n    {\n        return 2;\n    }\n}",
            ['base' => "class Base\n{\n    public function present()\n    {\n        return 1;\n    }\n}"]
        );

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * PHP validates `#[\Override]` against the real hierarchy, which it always
     * has. Zephir may not see an ancestor at compile time, and rejecting
     * correct code is worse than missing the diagnostic, so the check is
     * skipped rather than guessed.
     */
    public function testOverrideIsNotCheckedWhenTheParentIsInvisible(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attroverrideblind',
            "class Sample extends \\Some\\Unknown\\Thing\n{\n    #[\\Override]\n    public function maybe()\n    {\n        return 1;\n    }\n}"
        );

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * The same check applies to a top-level function, which has no declaring
     * class to name in the message.
     */
    public function testFailsWhenNoDiscardIsPutOnAVoidFunction(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attrfnnodiscard',
            "#[\\NoDiscard]\nfunction nothing() -> void\n{\n    return;\n}\n\nclass Sample\n{\n}"
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('A void function does not return a value', $stderr);
    }

    public function testFailsWhenNoDiscardIsPutOnAVoidMethod(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'attrnodiscard',
            "class Sample\n{\n    #[\\NoDiscard]\n    public function nothing() -> void\n    {\n    }\n}"
        );

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('#[\\NoDiscard] requires a return value', $stderr);
    }
}
