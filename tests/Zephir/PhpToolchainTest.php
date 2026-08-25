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

    /** @var list<string> */
    private array $linksToCleanup = [];

    protected function setUp(): void
    {
        $this->prefix = sys_get_temp_dir() . '/zephir-toolchain-' . uniqid('', true);
    }

    protected function tearDown(): void
    {
        foreach ($this->linksToCleanup as $link) {
            @unlink($link);
        }
        $this->linksToCleanup = [];

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

    public function testDefaultResolvesPhpConfigFromPath(): void
    {
        $this->assertSame('php-config', PhpToolchain::default()->phpConfigCommand());
    }

    /**
     * The pre-compiled header and the build fingerprint have to be derived
     * from the very same php-config `configure` is told about, otherwise they
     * describe a different PHP than the one the extension is built against.
     */
    public function testExplicitPhpConfigIsReportedForTheBuildEnvironment(): void
    {
        $this->skipOnWindows();
        $phpConfig = $this->createPrefix();

        $toolchain = PhpToolchain::fromPhpConfig($phpConfig);

        $this->assertSame(
            escapeshellarg($this->physicalPrefix() . '/php-config'),
            $toolchain->phpConfigCommand(),
        );
    }

    public function testExplicitPhpConfigPairsTheSiblingPhpize(): void
    {
        $this->skipOnWindows();
        $phpConfig = $this->createPrefix();

        $toolchain = PhpToolchain::fromPhpConfig($phpConfig);

        // Physical paths: the temporary directory is itself a symlink on macOS
        // (/var/folders -> /private/var/folders).
        $this->assertSame(
            escapeshellarg($this->physicalPrefix() . '/phpize'),
            $toolchain->phpizeCommand(),
        );
        $this->assertSame(
            ' --with-php-config=' . escapeshellarg($this->physicalPrefix() . '/php-config'),
            $toolchain->configureOption(),
        );
    }

    /**
     * `configure` runs from inside ext/ and bakes the given path into
     * ext/Makefile, so the toolchain must report where the tools physically
     * live rather than the symlink it was reached through.
     */
    public function testSymlinkedPrefixResolvesToThePhysicalPath(): void
    {
        $this->skipOnWindows();
        $this->createPrefix();

        $link = $this->prefix . '-link';
        symlink($this->prefix, $link);
        $this->linksToCleanup[] = $link;

        $toolchain = PhpToolchain::fromPhpConfig($link . '/php-config');

        $this->assertSame(
            escapeshellarg($this->physicalPrefix() . '/phpize'),
            $toolchain->phpizeCommand(),
        );
        $this->assertSame(
            ' --with-php-config=' . escapeshellarg($this->physicalPrefix() . '/php-config'),
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
            ' --with-php-config=' . escapeshellarg($this->physicalPrefix() . '/php-config'),
            $toolchain->configureOption(),
        );
        $this->assertSame(
            escapeshellarg($this->physicalPrefix() . '/phpize'),
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
        $this->expectExceptionMessage($this->physicalPrefix() . '/phpize');

        PhpToolchain::fromPhpConfig($phpConfig);
    }

    /**
     * On Windows the platform guard fires first for every value, so the
     * value-specific message only exists elsewhere. See
     * testEveryValueIsRejectedOnWindows for the Windows side.
     */
    public function testValuelessOptionIsRejected(): void
    {
        $this->skipOnWindows();

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('The "--with-php-config" option requires a value.');

        // Phalcon\Cop\Parser yields `true` for `--with-php-config` without a value.
        PhpToolchain::fromPhpConfig(true);
    }

    public function testEmptyValueIsRejected(): void
    {
        $this->skipOnWindows();

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('The "--with-php-config" option requires a value.');

        PhpToolchain::fromPhpConfig('   ');
    }

    /**
     * @dataProvider windowsValueProvider
     *
     * @param mixed $value
     */
    public function testEveryValueIsRejectedOnWindows($value): void
    {
        if (!Os::isWindows()) {
            $this->markTestSkipped('The Windows-only rejection path needs Windows.');
        }

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('PHP_DEVPACK');

        PhpToolchain::fromPhpConfig($value);
    }

    public function windowsValueProvider(): array
    {
        return [
            'existing path'  => ['C:\\php\\php-config'],
            'no value given' => [true],
            'empty value'    => ['   '],
        ];
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

    /**
     * The prefix with every symlink resolved, which is what the toolchain reports.
     */
    private function physicalPrefix(): string
    {
        return realpath($this->prefix) ?: $this->prefix;
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
