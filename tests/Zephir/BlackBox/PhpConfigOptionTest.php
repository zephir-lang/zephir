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
 * Builds against a PHP installation that is not the one in the PATH.
 *
 * The reporter of #1834 had a second (ZTS) PHP in /opt/php7 and got an
 * extension linked against the system PHP. A prefix holding logging wrappers
 * around the real php-config and phpize stands in for that second install
 * here: the build must consult the wrappers, not the PATH.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1834
 *
 * @group slow
 */
final class PhpConfigOptionTest extends TestCase
{
    use RunsZephirCommands;

    private string $shimPrefix = '';
    private string $shimLog    = '';

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('The --with-php-config option is rejected on Windows.');
        }

        $this->setUpZephirRunner();

        $this->shimPrefix = $this->outputDir() . '/php-config-shim';
        $this->shimLog    = $this->shimPrefix . '/calls.log';
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testCompileBuildsAgainstTheGivenPhpConfig(): void
    {
        $this->createShimPrefix();

        $cwd = $this->fixturesDir() . '/devmode';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $option = '--with-php-config=' . escapeshellarg($this->shimPrefix . '/php-config');

        // ext/ has to exist for fullclean to reach phpize --clean.
        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('fullclean ' . $option, $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);

        $compile = $this->runZephir('compile --no-dev ' . $option, $cwd);
        $this->assertSame(0, $compile['exitCode'], $compile['stderr']);

        // configure recorded the invocation it was given.
        $configNice = $cwd . '/ext/config.nice';
        $this->assertFileExists($configNice);
        $this->assertStringContainsString(
            '--with-php-config=' . $this->shimPrefix . '/php-config',
            (string) file_get_contents($configNice),
        );

        // ... and both wrappers were actually the tools that ran.
        $this->assertFileExists($this->shimLog);
        $log = (string) file_get_contents($this->shimLog);
        $this->assertStringContainsString('php-config --includes', $log);
        $this->assertStringContainsString('phpize --clean', $log);

        $this->assertFileExists($cwd . '/ext/modules/devmode.so');
    }

    public function testCompileRejectsAnUnusablePhpConfig(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';

        $result = $this->runZephir('compile --with-php-config=/nonexistent/bin/php-config', $cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('/nonexistent/bin/php-config', $result['stderr']);
    }

    public function testFullcleanRejectsAnUnusablePhpConfig(): void
    {
        $cwd = $this->fixturesDir() . '/devmode';

        $result = $this->runZephir('fullclean --with-php-config=/nonexistent/bin/php-config', $cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('/nonexistent/bin/php-config', $result['stderr']);
    }

    public function testCompileRejectsAPrefixWithoutPhpize(): void
    {
        $prefix = $this->outputDir() . '/php-config-lonely';
        $this->cleanupPath($prefix);
        mkdir($prefix, 0755, true);
        file_put_contents($prefix . '/php-config', "#!/bin/sh\nexit 0\n");
        chmod($prefix . '/php-config', 0755);

        $result = $this->runZephir(
            'compile --with-php-config=' . escapeshellarg($prefix . '/php-config'),
            $this->fixturesDir() . '/devmode',
        );

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString($prefix . '/phpize', $result['stderr']);
    }

    /**
     * Creates a PHP prefix whose php-config and phpize log their arguments and
     * then hand over to the real tools.
     */
    private function createShimPrefix(): void
    {
        $this->cleanupPath($this->shimPrefix);
        $this->removePath($this->shimPrefix);
        mkdir($this->shimPrefix, 0755, true);

        foreach (['php-config', 'phpize'] as $tool) {
            $real = $this->locateTool($tool);
            $path = $this->shimPrefix . '/' . $tool;

            file_put_contents($path, sprintf(
                "#!/bin/sh\necho \"%s \$*\" >> %s\nexec %s \"\$@\"\n",
                $tool,
                escapeshellarg($this->shimLog),
                escapeshellarg($real),
            ));
            chmod($path, 0755);
        }
    }

    /**
     * Finds the real build tool next to the running PHP, falling back to the PATH.
     */
    private function locateTool(string $tool): string
    {
        $candidate = dirname(PHP_BINARY) . '/' . $tool;
        if (is_file($candidate) && is_executable($candidate)) {
            return $candidate;
        }

        $found = trim((string) shell_exec('command -v ' . escapeshellarg($tool) . ' 2>/dev/null'));
        if ($found === '' || !is_executable($found)) {
            $this->markTestSkipped(sprintf('%s is not available, cannot build a stand-in prefix.', $tool));
        }

        return $found;
    }
}
