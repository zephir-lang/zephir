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

namespace Zephir\Test\Compiler;

use PHPUnit\Framework\TestCase;
use Psr\Log\Test\TestLogger;
use ReflectionMethod;
use Zephir\Backend\Backend;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Covers what `zephir install` reports as the install destination.
 *
 * `configure` bakes `php-config --extension-dir` of the PHP the extension was
 * built against into `ext/Makefile`, so that is where the destination comes
 * from — not a fresh `php-config` lookup, which would name the wrong PHP for a
 * build retargeted with `--with-php-config`. The installed file is confirmed to
 * be there rather than assumed, because `make install` runs through `sudo` and
 * its exit code is not what `install()` checks.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2467
 */
final class ExtensionInstallDirTest extends TestCase
{
    private string $originalCwd;
    private string $projectDir;
    private string $destination;

    protected function setUp(): void
    {
        $this->originalCwd = getcwd();
        $this->projectDir  = sys_get_temp_dir() . '/zephir-install-' . uniqid('', true);
        $this->destination = $this->projectDir . '/dest';

        mkdir($this->projectDir . '/ext', 0755, true);
        mkdir($this->destination, 0755, true);

        // The destination is read from ext/Makefile relative to the CWD.
        chdir($this->projectDir);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);
        $this->removeRecursively($this->projectDir);
    }

    public function testReportsTheDirectoryTheExtensionWasInstalledInto(): void
    {
        $this->writeMakefile();
        $this->writeInstalledExtension('test');

        $this->assertSame($this->destination, $this->resolve($this->createCompiler()));
    }

    public function testReportsNothingWhenTheProjectWasNeverConfigured(): void
    {
        $this->writeInstalledExtension('test');

        $this->assertNull($this->resolve($this->createCompiler()));
    }

    public function testReportsNothingWhenTheMakefileDoesNotNameTheDestination(): void
    {
        file_put_contents($this->projectDir . '/ext/Makefile', "CC = gcc\n");
        $this->writeInstalledExtension('test');

        $this->assertNull($this->resolve($this->createCompiler()));
    }

    /**
     * `sudo make install` may not have gone through — its exit code is not what
     * install() checks. Naming a directory the extension is not in would be a
     * worse answer than naming none.
     */
    public function testReportsNothingWhenTheExtensionIsNotAtTheDestination(): void
    {
        $this->writeMakefile();

        $this->assertNull($this->resolve($this->createCompiler()));
    }

    public function testUsesTheConfiguredExtensionNameForTheInstalledFile(): void
    {
        $this->writeMakefile();
        $this->writeInstalledExtension('custom');

        $this->assertSame($this->destination, $this->resolve($this->createCompiler('custom')));
    }

    public function testExtensionFileNameFallsBackToTheNamespace(): void
    {
        $this->assertSame('test', $this->createCompiler()->getExtensionFileName());
    }

    public function testExtensionFileNameIsTheConfiguredOne(): void
    {
        $this->assertSame('custom', $this->createCompiler('custom')->getExtensionFileName());
    }

    private function resolve(Compiler $compiler): ?string
    {
        $method = new ReflectionMethod(Compiler::class, 'resolveExtensionInstallDir');
        $method->setAccessible(true);

        return $method->invoke($compiler, $this->projectDir, $compiler->getExtensionFileName());
    }

    private function writeMakefile(): void
    {
        file_put_contents(
            $this->projectDir . '/ext/Makefile',
            "CC = gcc\nEXTENSION_DIR = {$this->destination}\n"
        );
    }

    private function writeInstalledExtension(string $fileName): void
    {
        file_put_contents($this->destination . '/' . $fileName . '.so', 'ELF');
    }

    private function createCompiler(?string $extensionName = null): Compiler
    {
        $logger = new TestLogger();
        $config = new Config();
        $config->set('namespace', 'test');
        if (null !== $extensionName) {
            $config->set('extension-name', $extensionName);
        }

        $backend = new Backend($config, 'kernel', 'templates');
        $disk    = new HardDisk($this->projectDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');

        $compiler = new Compiler(
            $config,
            $backend,
            new Manager(new Parser()),
            $disk,
            new CompilerFileFactory($config, $disk, $logger)
        );
        $compiler->setLogger($logger);

        return $compiler;
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
