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
 * Equivalent of sharness `t0006-info-commands.sh`.
 */
final class InfoCommandsTest extends TestCase
{
    use RunsZephirCommands;

    private string $cwd;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->cwd = sys_get_temp_dir() . '/zephir-info-' . bin2hex(random_bytes(6));
        mkdir($this->cwd, 0777, true);
        $this->cleanupPath($this->cwd);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testVersionFlagsDoNotCreateZephirCache(): void
    {
        $this->assertSame(0, $this->runZephir('-V', $this->cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('--version', $this->cwd)['exitCode']);
        $this->assertDirectoryDoesNotExist($this->cwd . '/.zephir');
    }

    public function testDumpVersionDoesNotCreateZephirCache(): void
    {
        $this->assertSame(0, $this->runZephir('--dumpversion', $this->cwd)['exitCode']);
        $this->assertDirectoryDoesNotExist($this->cwd . '/.zephir');
    }

    public function testHelpCommandsDoNotCreateZephirCache(): void
    {
        $this->assertSame(0, $this->runZephir('', $this->cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('help', $this->cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('-h', $this->cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('--help', $this->cwd)['exitCode']);
        $this->assertDirectoryDoesNotExist($this->cwd . '/.zephir');
    }
}
