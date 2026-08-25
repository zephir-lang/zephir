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
 * `zephir build` names the directory it installed the extension into.
 *
 * The reporter of #2467 had a working "hello world" extension and no way to
 * tell where `build` had put it, which is what loading it by hand with
 * `php -d extension=...` needs.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2467
 *
 * @group slow
 */
final class InstallPathTest extends TestCase
{
    use RunsZephirCommands;

    private string $extensionDir  = '';
    private string $installedFile = '';
    private bool $preExisting     = false;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Installation is not implemented for Windows.');
        }

        // `install()` shells out to `sudo make install`. Without a sudo that
        // runs unattended the command would sit on a password prompt.
        exec('sudo -n true 2>/dev/null', $output, $exit);
        if (0 !== $exit) {
            $this->markTestSkipped('Installing the extension requires passwordless sudo.');
        }

        $this->setUpZephirRunner();

        $this->extensionDir = trim((string)shell_exec('php-config --extension-dir 2>/dev/null'));
        if ('' === $this->extensionDir || !is_dir($this->extensionDir)) {
            $this->markTestSkipped('The PHP extension directory could not be determined.');
        }

        $this->installedFile = $this->extensionDir . '/devmode.so';
        $this->preExisting   = is_file($this->installedFile);
    }

    protected function tearDown(): void
    {
        // Leave the extension directory as it was found.
        if (!$this->preExisting && '' !== $this->installedFile && is_file($this->installedFile)) {
            if (!@unlink($this->installedFile)) {
                exec('sudo -n rm -f ' . escapeshellarg($this->installedFile) . ' 2>/dev/null');
            }
        }

        $this->tearDownZephirRunner();
    }

    public function testBuildReportsTheDirectoryTheExtensionWasInstalledInto(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $build = $this->runZephir('build --no-dev', $cwd);

        $this->assertSame(0, $build['exitCode'], $build['stderr']);
        $this->assertStringContainsString(
            'Extension installed into ' . $this->extensionDir,
            $build['stdout'],
        );
    }

    /**
     * The reported directory is where the extension actually is, not where the
     * Makefile says it should be.
     */
    public function testTheReportedDirectoryHoldsTheExtension(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $build = $this->runZephir('build --no-dev', $cwd);
        $this->assertSame(0, $build['exitCode'], $build['stderr']);

        if (!preg_match('/^Extension installed into (.+)$/m', $build['stdout'], $matches)) {
            $this->fail('build did not report an install directory: ' . $build['stdout']);
        }

        $this->assertFileExists(trim($matches[1]) . '/devmode.so');
    }
}
