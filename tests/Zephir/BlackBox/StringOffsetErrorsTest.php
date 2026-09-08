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
 * Operations PHP rejects at runtime on a string are rejected at build time
 * when the Zephir source declares the variable `string`.
 *
 * `$s[] = 'x'`, `unset($s[0])` and `$s[0][1] = 'x'` are all `Error`s in PHP.
 * On a `var` the compiler cannot know, so the same `Error` is raised at
 * runtime and covered by Issue760Test. On a declared `string` the operation
 * could never have succeeded, so failing the build is both earlier and more
 * useful than emitting code that is guaranteed to throw.
 *
 * @see https://github.com/zephir-lang/zephir/issues/760
 */
final class StringOffsetErrorsTest extends TestCase
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

    public function testAppendingToAStringIsRejected(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'soappend',
            "    public function run(string s)\n    {\n        let s[] = \"x\";\n    }"
        );

        $this->assertSame(1, $exitCode, $stderr);
        $this->assertStringContainsString('as an array', $stderr);
    }

    public function testUnsettingAStringOffsetIsRejected(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'sounset',
            "    public function run(string s)\n    {\n        unset s[0];\n    }"
        );

        $this->assertSame(1, $exitCode, $stderr);
        $this->assertStringContainsString('in "unset"', $stderr);
    }

    public function testChainedStringOffsetWriteIsRejected(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'sochained',
            "    public function run(string s)\n    {\n        let s[0][1] = \"x\";\n    }"
        );

        $this->assertSame(1, $exitCode, $stderr);
        $this->assertStringContainsString('Cannot use string offset as an array', $stderr);
    }

    /**
     * The positive control: a plain string-offset write must still build, so a
     * green run above cannot be the compiler rejecting everything.
     */
    public function testPlainStringOffsetWriteStillCompiles(): void
    {
        [$exitCode, $stderr] = $this->generateProject(
            'sowrite',
            "    public function run(string s) -> string\n    {\n        let s[0] = \"x\";\n        return s;\n    }"
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
