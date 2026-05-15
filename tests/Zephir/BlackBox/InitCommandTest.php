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
 * Equivalent of sharness `t0003-init-errors.sh` and `t0003-init-success.sh`.
 */
final class InitCommandTest extends TestCase
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

    public function testInitFailsWhenNamespaceMissing(): void
    {
        $result = $this->runZephir('init', $this->outputDir());

        $this->assertSame(1, $result['exitCode']);
        $this->assertSame(
            'Not enough arguments (missing: "namespace").',
            trim($result['stderr']),
        );
    }

    public function testVersionFlagDoesNotDumpConfig(): void
    {
        $cwd = $this->outputDir();
        $configPath = $cwd . '/config.json';
        $this->cleanupPath($configPath);

        $result = $this->runZephir('--version -Wunused-variable', $cwd);

        $this->assertSame(0, $result['exitCode']);
        $this->assertFileDoesNotExist($configPath);
    }

    public function testInitGeneratesProjectAndDumpsConfig(): void
    {
        $cwd = $this->outputDir();
        $projectDir = $cwd . '/success';
        $this->cleanupPath($projectDir);

        $result = $this->runZephir('init success', $cwd);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertDirectoryExists($projectDir);
        $this->assertDirectoryExists($projectDir . '/success');
        $this->assertDirectoryExists($projectDir . '/ext');
        $this->assertFileExists($projectDir . '/config.json');
    }
}
