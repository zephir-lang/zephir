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
 * Equivalent of sharness `t0001-compile.sh`.
 *
 * @group slow
 */
final class CompileCommandTest extends TestCase
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

    public function testCompileExtensionInProductionMode(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $this->assertSame(0, $this->runZephir('fullclean', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('compile --no-dev', $cwd)['exitCode']);

        $configNice = $cwd . '/ext/config.nice';
        $this->assertFileExists($configNice);
        $contents = file_get_contents($configNice);
        $this->assertMatchesRegularExpression(
            "/^CFLAGS='-O2 -fvisibility=hidden -Wparentheses( -flto)? -DZEPHIR_RELEASE=1'/m",
            $contents,
        );
    }

    public function testCompileExtensionInDevelopmentMode(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $this->assertSame(0, $this->runZephir('fullclean', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('compile --dev', $cwd)['exitCode']);

        $configNice = $cwd . '/ext/config.nice';
        $this->assertFileExists($configNice);
        $contents = file_get_contents($configNice);
        $this->assertMatchesRegularExpression(
            "/^CFLAGS='-O0 -g3'/m",
            $contents,
        );
    }
}
