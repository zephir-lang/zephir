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
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, string $body): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }
}
