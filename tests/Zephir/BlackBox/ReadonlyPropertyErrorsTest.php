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
 * Compile-time validation of readonly-property misuse (issue #2614): each case
 * here is a fatal error in PHP 8.1+, so the Zephir compiler must reject it
 * during `generate` instead of producing a broken extension. A valid readonly
 * property (typed, no default, non-static) must still compile.
 *
 * The pure-PHP parser is forced so the check is deterministic and does not
 * depend on the readonly grammar being present in the C extension.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2614
 */
final class ReadonlyPropertyErrorsTest extends TestCase
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
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, string $body): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = "namespace " . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }

    public function testFailsWhenReadonlyHasDefaultValue(): void
    {
        [$exitCode, $stderr] = $this->generateProject('rodefault', "    public readonly array foo = [];");

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Readonly property', $stderr);
        $this->assertStringContainsString('cannot have a default value', $stderr);
    }

    public function testFailsWhenReadonlyIsUntyped(): void
    {
        [$exitCode, $stderr] = $this->generateProject('rountyped', "    public readonly foo;");

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Readonly property', $stderr);
        $this->assertStringContainsString('must have a type', $stderr);
    }

    public function testFailsWhenReadonlyIsStatic(): void
    {
        [$exitCode, $stderr] = $this->generateProject('rostatic', "    public static readonly int foo;");

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Readonly property', $stderr);
        $this->assertStringContainsString('cannot be static', $stderr);
    }

    public function testAllowsValidReadonlyProperty(): void
    {
        [$exitCode, $stderr] = $this->generateProject('rovalid', "    public readonly array foo;");

        $this->assertSame(0, $exitCode, $stderr);
    }
}
