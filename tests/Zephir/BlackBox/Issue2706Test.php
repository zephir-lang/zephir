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
 * `arrayIssetFetch()` looked up a literal key it has no branch for as a
 * variable, without a source location, so `fetch x, a[1.5]` killed the
 * compiler with a PHP TypeError from CompilerException's constructor.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
final class Issue2706Test extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testAnUnsupportedFetchKeyIsACompileErrorNotACrash(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'fetchchar',
            "    public function run(array a)\n    {\n        var x;\n\n        return fetch x, a['c'];\n    }"
        );

        $this->assertSame(1, $exitCode, $stderr);
        $this->assertStringNotContainsString('TypeError', $stderr);
        $this->assertStringContainsString('sample.zep', $stderr);
    }

    /**
     * The float, bool and null keys that used to crash the same way now
     * compile: they are boxed and converted by the kernel as PHP converts them.
     */
    public function testFloatBoolAndNullLiteralFetchKeysCompile(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'fetchscalar',
            "    public function run(array a) -> array\n    {\n        var x, y;\n\n"
            . "        return [fetch x, a[1.5], fetch y, a[true], fetch x, a[null]];\n    }"
        );

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * PHP deprecates a fractional float key when it evaluates a declaration.
     * The extension folds declarations at compile time, so the compiler
     * reports each lossy key instead; a whole float key and a literal in a
     * method body, which still deprecates at run time, are not reported.
     */
    public function testLossyFloatKeysInDeclarationsAreReported(): void
    {
        [$exitCode, $stderr] = $this->generateProject('lossykeys', self::LOSSY_DECLARATIONS);

        $this->assertSame(0, $exitCode, $stderr);
        foreach (['1.5', '2.5', '3.5', '4.5', '5.5'] as $float) {
            $this->assertSame(
                1,
                substr_count($stderr, 'Implicit conversion from float ' . $float . ' to int loses precision'),
                $stderr
            );
        }
        $this->assertStringNotContainsString('float 2.0 ', $stderr);
        $this->assertStringNotContainsString('float 6.5 ', $stderr);
        $this->assertStringContainsString('[lossy-float-key]', $stderr);
    }

    public function testTheLossyFloatKeyWarningCanBeDisabled(): void
    {
        [$exitCode, $stderr] = $this->generateProject('lossyoff', self::LOSSY_DECLARATIONS, ' -Wlossy-float-key');

        $this->assertSame(0, $exitCode, $stderr);
        $this->assertStringNotContainsString('loses precision', $stderr);
    }

    private const LOSSY_DECLARATIONS = <<<'ZEP'
            const C = [1.5: "a", 2.0: "b", "x": [2.5: "c"]];

            public p = [3.5: "a"];

            public static sp = [4.5: "a"];

            protected array tp = [5.5: "a"];

            public function run() -> array
            {
                return [6.5: "a"];
            }
        ZEP;

    /**
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, string $body, string $flags = ''): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi' . $flags, $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }
}
