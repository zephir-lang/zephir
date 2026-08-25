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
use Zephir\Os;

/**
 * A `make install` that did not go through is reported as a failure.
 *
 * `install()` copies the module with `sudo make install` but used to check only
 * that `make` had produced `ext/modules/<name>.so`, never the exit status. A
 * container without sudo, a declined password or a read-only extension
 * directory therefore ended in `Extension installed.` and a zero exit code with
 * nothing installed.
 *
 * A `sudo` that always fails, placed ahead of the real one in the PATH, stands
 * in for all three.
 *
 * @group slow
 */
final class InstallFailureTest extends TestCase
{
    use RunsZephirCommands;

    private string $originalPath = '';

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Installation is not implemented for Windows.');
        }

        $this->setUpZephirRunner();
        $this->originalPath = (string)getenv('PATH');
    }

    protected function tearDown(): void
    {
        if ('' !== $this->originalPath) {
            putenv('PATH=' . $this->originalPath);
        }

        $this->tearDownZephirRunner();
    }

    public function testInstallFailsWhenTheCopyIntoTheExtensionDirectoryIsRefused(): void
    {
        $result = $this->runInstallWithAFailingSudo('install');

        $this->assertSame(1, $result['exitCode'], $result['stdout']);
    }

    public function testInstallDoesNotClaimSuccessWhenNothingWasInstalled(): void
    {
        $result = $this->runInstallWithAFailingSudo('install');

        $this->assertStringNotContainsString('Extension installed', $result['stdout']);
    }

    public function testInstallSaysWhatWentWrong(): void
    {
        $result = $this->runInstallWithAFailingSudo('install');

        $this->assertStringContainsString('exit code', $result['stderr']);
    }

    /**
     * `build` is generate + compile + install, and swallowed the same failure.
     */
    public function testBuildFailsWhenInstallingFails(): void
    {
        $result = $this->runInstallWithAFailingSudo('build --no-dev');

        $this->assertSame(1, $result['exitCode'], $result['stdout']);
    }

    /**
     * Compiles the fixture with the real toolchain, then runs the given command
     * with a `sudo` that refuses, so that only the install step can fail.
     *
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function runInstallWithAFailingSudo(string $command): array
    {
        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);

        $compile = $this->runZephir('compile --no-dev', $cwd);
        $this->assertSame(0, $compile['exitCode'], $compile['stderr']);

        putenv('PATH=' . $this->createFailingSudo() . ':' . $this->originalPath);

        return $this->runZephir($command, $cwd);
    }

    /**
     * A prefix whose `sudo` refuses the way a missing password does.
     */
    private function createFailingSudo(): string
    {
        $prefix = $this->outputDir() . '/failing-sudo';
        $this->cleanupPath($prefix);
        $this->removePath($prefix);
        mkdir($prefix, 0755, true);

        $sudo = $prefix . '/sudo';
        file_put_contents($sudo, "#!/bin/sh\necho 'sudo: a password is required' >&2\nexit 1\n");
        chmod($sudo, 0755);

        return $prefix;
    }
}
