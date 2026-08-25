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
use Zephir\PhpToolchain;
use Zephir\Zephir;

/**
 * Covers when compile() has to re-run phpize/configure, which wipes every
 * object file with it. Reusing the existing Makefile is what makes a rebuild
 * after a one-file change cost a single translation unit instead of all of
 * them, so the conditions have to be exact in both directions.
 */
final class NeedsConfigureTest extends TestCase
{
    private string $originalCwd;
    private string $projectDir;

    protected function setUp(): void
    {
        $this->originalCwd = getcwd();
        $this->projectDir  = sys_get_temp_dir() . '/zephir-configure-' . uniqid('', true);
        mkdir($this->projectDir . '/ext', 0755, true);
        // needsConfigure() reads ext/Makefile and ext/config.h relative to the CWD.
        chdir($this->projectDir);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);
        $this->removeRecursively($this->projectDir);
    }

    public function testConfiguresWhenTheProjectWasNeverPhpized(): void
    {
        $compiler = $this->createCompiler();

        $this->assertTrue($this->needsConfigure($compiler));
    }

    /**
     * `zephir generate` and `zephir compile` are separate processes, so the
     * decision has to survive as a file rather than as in-memory state.
     */
    public function testConfiguresWhenGenerateReportedAChangedConfiguration(): void
    {
        $this->markConfigured();
        $compiler = $this->createCompiler();
        $this->rememberFingerprint($compiler);
        file_put_contents($this->cacheEntry(Compiler::NEEDS_CONFIGURE), '1');

        $this->assertTrue($this->needsConfigure($compiler));
    }

    /**
     * A Makefile without config.h is a configure run that died halfway.
     */
    public function testConfiguresWhenConfigureNeverFinished(): void
    {
        $this->markConfigured();
        unlink($this->projectDir . '/ext/config.h');
        $compiler = $this->createCompiler();
        $this->rememberFingerprint($compiler);

        $this->assertTrue($this->needsConfigure($compiler));
    }

    public function testConfiguresWhenNothingIsKnownAboutTheExistingBuild(): void
    {
        $this->markConfigured();
        $compiler = $this->createCompiler();

        $this->assertTrue($this->needsConfigure($compiler));
    }

    /**
     * The objects on disk were built for another PHP, another php-config or
     * other flags. Six PHP containers sharing one project directory is a
     * normal setup, and linking their objects together produces a broken .so.
     */
    public function testConfiguresWhenTheBuildEnvironmentChanged(): void
    {
        $this->markConfigured();
        $compiler = $this->createCompiler();
        // Fingerprint of a debug build, while a release build is requested.
        $this->rememberFingerprint($compiler, true);

        $this->assertTrue($this->needsConfigure($compiler, false));
    }

    public function testReusesTheExistingMakefileWhenNothingChanged(): void
    {
        $this->markConfigured();
        $compiler = $this->createCompiler();
        $this->rememberFingerprint($compiler);

        $this->assertFalse($this->needsConfigure($compiler));
    }

    public function testFingerprintIsStableForTheSameBuild(): void
    {
        $parts = ['gcc-flags' => '-O2', 'development' => false];

        $this->assertSame(Compiler::fingerprintOf($parts), Compiler::fingerprintOf($parts));
    }

    public function testFingerprintChangesWithTheBuildFlags(): void
    {
        $this->assertNotSame(
            Compiler::fingerprintOf(['gcc-flags' => '-O2']),
            Compiler::fingerprintOf(['gcc-flags' => '-O0 -g3'])
        );
    }

    /**
     * The fingerprint is only usable when the flags going into it do not drift
     * between two builds of the same project, which is what a cached raw
     * `gcc -dumpversion` used to do.
     */
    public function testGccVersionParsesTheSameCachedAndFresh(): void
    {
        $compiler = $this->createCompiler();

        $this->assertSame($compiler->getGccFlags(), $compiler->getGccFlags());
    }

    public function testGccVersionIsUnreadFromMajorOnlyOutput(): void
    {
        // What `gcc -dumpversion` prints since GCC 7.
        $this->assertSame('0.0.0', Compiler::parseGccVersion(['14', '']));
    }

    public function testGccVersionIsReadFromFullOutput(): void
    {
        $this->assertSame('14.2.0', Compiler::parseGccVersion(['14.2.0', '']));
    }

    public function testGccVersionFallsBackWhenNothingWasCaptured(): void
    {
        $this->assertSame('0.0.0', Compiler::parseGccVersion([]));
    }

    private function markConfigured(): void
    {
        file_put_contents($this->projectDir . '/ext/Makefile', "# configured\n");
        file_put_contents($this->projectDir . '/ext/config.h', "/* configured */\n");
    }

    private function needsConfigure(Compiler $compiler, bool $development = false): bool
    {
        $method = new ReflectionMethod(Compiler::class, 'needsConfigure');
        $method->setAccessible(true);

        return $method->invoke($compiler, $development, PhpToolchain::default());
    }

    /**
     * Stores the fingerprint of the given build, the way a successful
     * configure run does.
     */
    private function rememberFingerprint(Compiler $compiler, bool $development = false): void
    {
        $method = new ReflectionMethod(Compiler::class, 'buildFingerprint');
        $method->setAccessible(true);

        $fingerprint = $method->invoke($compiler, $development, PhpToolchain::default());

        file_put_contents($this->cacheEntry(Compiler::BUILD_FINGERPRINT), $fingerprint);
    }

    private function cacheEntry(string $name): string
    {
        return $this->projectDir . '/.zephir/' . Zephir::VERSION . '/' . $name;
    }

    private function createCompiler(): Compiler
    {
        $logger = new TestLogger();
        $config = new Config();
        $config->set('namespace', 'test');
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
