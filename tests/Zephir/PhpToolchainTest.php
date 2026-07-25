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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Exception\CompilerException;
use Zephir\Os;
use Zephir\PhpToolchain;

/**
 * Covers the resolution of the `--with-php-config` build option.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1834
 */
final class PhpToolchainTest extends TestCase
{
    private string $prefix = '';

    protected function setUp(): void
    {
        $this->prefix = sys_get_temp_dir() . '/zephir-toolchain-' . uniqid('', true);
    }

    protected function tearDown(): void
    {
        if ($this->prefix !== '') {
            $this->removeRecursively($this->prefix);
        }
    }

    public function testDefaultResolvesPhpizeFromPath(): void
    {
        $toolchain = PhpToolchain::default();

        $this->assertSame('phpize', $toolchain->phpizeCommand());
        $this->assertSame('', $toolchain->configureOption());
    }

    public function testNullPathBehavesLikeDefault(): void
    {
        $toolchain = PhpToolchain::fromPhpConfig(null);

        $this->assertSame('phpize', $toolchain->phpizeCommand());
        $this->assertSame('', $toolchain->configureOption());
    }

    public function testExplicitPhpConfigPairsTheSiblingPhpize(): void
    {
        $this->skipOnWindows();
        $phpConfig = $this->createPrefix();

        $toolchain = PhpToolchain::fromPhpConfig($phpConfig);

        $this->assertSame(escapeshellarg($this->prefix . '/phpize'), $toolchain->phpizeCommand());
        $this->assertSame(
            ' --with-php-config=' . escapeshellarg($phpConfig),
            $toolchain->configureOption(),
        );
    }

    /**
     * `configure` runs from inside the ext/ directory, so a relative path given
     * on the command line has to be made absolute before it is handed over.
     */
    public function testRelativePathIsResolvedToAnAbsolutePath(): void
    {
        $this->skipOnWindows();
        $this->createPrefix();

        $originalCwd = getcwd();
        chdir($this->prefix);

        try {
            $toolchain = PhpToolchain::fromPhpConfig('./php-config');
        } finally {
            chdir($originalCwd);
        }

        $this->assertSame(
            ' --with-php-config=' . escapeshellarg(realpath($this->prefix) . '/php-config'),
            $toolchain->configureOption(),
        );
        $this->assertSame(
            escapeshellarg(realpath($this->prefix) . '/phpize'),
            $toolchain->phpizeCommand(),
        );
    }

    public function testPathContainingSpacesIsQuoted(): void
    {
        $this->skipOnWindows();
        $this->prefix .= ' with space';
        $phpConfig = $this->createPrefix();

        $toolchain = PhpToolchain::fromPhpConfig($phpConfig);

        $this->assertStringContainsString("'", $toolchain->configureOption());
        $this->assertStringContainsString("'", $toolchain->phpizeCommand());
    }

    public function testMissingPhpConfigIsRejected(): void
    {
        $this->skipOnWindows();

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('/nonexistent/bin/php-config');

        PhpToolchain::fromPhpConfig('/nonexistent/bin/php-config');
    }

    public function testDirectoryInsteadOfPhpConfigIsRejected(): void
    {
        $this->skipOnWindows();
        mkdir($this->prefix, 0755, true);

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage($this->prefix);

        PhpToolchain::fromPhpConfig($this->prefix);
    }

    public function testNonExecutablePhpConfigIsRejected(): void
    {
        $this->skipOnWindows();
        $phpConfig = $this->createPrefix();
        chmod($phpConfig, 0644);

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage($phpConfig);

        PhpToolchain::fromPhpConfig($phpConfig);
    }

    public function testMissingSiblingPhpizeIsRejected(): void
    {
        $this->skipOnWindows();
        $phpConfig = $this->createPrefix();
        unlink($this->prefix . '/phpize');

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage($this->prefix . '/phpize');

        PhpToolchain::fromPhpConfig($phpConfig);
    }

    public function testValuelessOptionIsRejected(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('The "--with-php-config" option requires a value.');

        // Phalcon\Cop\Parser yields `true` for `--with-php-config` without a value.
        PhpToolchain::fromPhpConfig(true);
    }

    public function testEmptyValueIsRejected(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('The "--with-php-config" option requires a value.');

        PhpToolchain::fromPhpConfig('   ');
    }

    public function testOptionIsRejectedOnWindows(): void
    {
        if (!Os::isWindows()) {
            $this->markTestSkipped('The Windows-only rejection path needs Windows.');
        }

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('PHP_DEVPACK');

        PhpToolchain::fromPhpConfig('C:\\php\\php-config');
    }

    /**
     * Creates a fake PHP prefix holding executable `php-config` and `phpize`.
     *
     * @return string Path to the fake php-config.
     */
    private function createPrefix(): string
    {
        mkdir($this->prefix, 0755, true);

        foreach (['php-config', 'phpize'] as $tool) {
            $path = $this->prefix . '/' . $tool;
            file_put_contents($path, "#!/bin/sh\nexit 0\n");
            chmod($path, 0755);
        }

        return $this->prefix . '/php-config';
    }

    private function skipOnWindows(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('The option is rejected on Windows, so path pairing cannot be tested.');
        }
    }

    private function removeRecursively(string $path): void
    {
        if (!is_dir($path)) {
            return;
        }

        foreach (scandir($path) as $entry) {
            if ('.' === $entry || '..' === $entry) {
                continue;
            }
            $full = $path . DIRECTORY_SEPARATOR . $entry;
            is_dir($full) ? $this->removeRecursively($full) : @unlink($full);
        }

        @rmdir($path);
    }
}
