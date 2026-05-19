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
 * Equivalent of sharness `t0005-stubs.sh`.
 *
 * Runs `generate` + `stubs` once and asserts all expected stub files.
 */
final class StubsCommandTest extends TestCase
{
    use RunsZephirCommands;

    private string $stubsCwd;
    private string $stubsOutDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
        $this->stubsCwd = $this->fixturesDir() . '/stubs/issues';
        $this->stubsOutDir = $this->stubsCwd . '/ide/0.0.1/Stubs';

        $this->cleanupPath($this->stubsCwd . '/.zephir');
        $this->cleanupPath($this->stubsCwd . '/ext');
        $this->cleanupPath($this->stubsCwd . '/ide');

        $this->removePath($this->stubsCwd . '/.zephir');
        $this->removePath($this->stubsCwd . '/ext');
        $this->removePath($this->stubsCwd . '/ide');

        $generate = $this->runZephir('generate --no-ansi', $this->stubsCwd);
        $this->assertSame(0, $generate['exitCode'], $generate['stderr']);

        $stubs = $this->runZephir('stubs --no-ansi', $this->stubsCwd);
        $this->assertSame(0, $stubs['exitCode'], $stubs['stderr']);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testStubsDirectoryExists(): void
    {
        $this->assertDirectoryExists($this->stubsOutDir);
    }

    /**
     * @dataProvider stubFileProvider
     */
    public function testGeneratedStubMatchesExpected(string $fileName): void
    {
        $expected = $this->stubsCwd . '/expected/' . $fileName;
        $actual = $this->stubsOutDir . '/' . $fileName;

        $this->assertFileExists($actual);
        $this->assertSame(
            $this->readNormalized($expected),
            $this->readNormalized($actual),
        );
    }

    /**
     * Reads a file and normalizes line endings to LF.
     *
     * Windows checkouts may convert fixture line endings to CRLF, while the
     * stub generator emits some sections (e.g. docblocks) with LF regardless
     * of platform. Compare on normalized content so the assertion is stable.
     */
    private function readNormalized(string $path): string
    {
        return str_replace("\r\n", "\n", (string) file_get_contents($path));
    }

    public static function stubFileProvider(): array
    {
        return [
            'Issue 2426 nullable params' => ['Issue_2426.php'],
            'Issue 1922 type hint' => ['Issue_1922.php'],
            'Issue 1778 namespace imports' => ['Issue_1778.php'],
            'Issue 1900 return types' => ['Issue_1900.php'],
            'Issue 1907 Exception namespace' => ['Exception.php'],
            'Issue 1907 namespace for extends (slash)' => ['Issue_1907.php'],
            'Issue 1986 aliases for use statements' => ['Issue_1986.php'],
            'Issue 2026 return type for Collections' => ['Issue_2026.php'],
            'Issue 2092 return type for type-hinted object' => ['Issue_2092.php'],
        ];
    }

    public function testCamelCaseFolderForEventsManagerInterface(): void
    {
        $this->assertFileExists($this->stubsOutDir . '/Events/ManagerInterface.php');
    }
}
