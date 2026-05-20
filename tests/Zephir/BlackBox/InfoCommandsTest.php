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

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2454
     */
    public function testVernumPrintsZeroPaddedNumericVersion(): void
    {
        $result = $this->runZephir('--vernum', $this->cwd);

        $this->assertSame(0, $result['exitCode']);

        // Format: %01d%02d%02d, zero-padded MAJOR(1) + MINOR(2) + PATCH(2).
        // For 0.21.0 the output is "02100". Avoid hard-coding the exact
        // value so the test survives version bumps; assert shape, and that
        // it round-trips to the current Zephir::VERSION major.minor.patch.
        $stdout = trim($result['stdout']);
        $this->assertMatchesRegularExpression('/^\d{5}$/', $stdout, "stdout was: '$stdout'");

        $declared = explode('-', \Zephir\Zephir::VERSION)[0];
        [$major, $minor, $patch] = array_pad(explode('.', $declared), 3, '0');
        $expected = sprintf('%01d%02d%02d', (int) $major, (int) $minor, (int) $patch);
        $this->assertSame($expected, $stdout);

        $this->assertDirectoryDoesNotExist($this->cwd . '/.zephir');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2454
     */
    public function testVernumDoesNotPrintHelpScreen(): void
    {
        $result = $this->runZephir('--vernum', $this->cwd);

        // Help text contains "Usage:" — --vernum must not fall through to it.
        $this->assertStringNotContainsString('Usage:', $result['stdout']);
        $this->assertStringNotContainsString('Available commands:', $result['stdout']);
    }
}
