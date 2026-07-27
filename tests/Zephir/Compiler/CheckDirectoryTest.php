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
use Zephir\Zephir;

/**
 * Covers the "Zephir version has changed" detection in Compiler::checkDirectory().
 *
 * @see https://github.com/zephir-lang/zephir/issues/2621
 */
final class CheckDirectoryTest extends TestCase
{
    private string $originalCwd;
    private string $projectDir;

    protected function setUp(): void
    {
        $this->originalCwd = getcwd();
        $this->projectDir  = sys_get_temp_dir() . '/zephir-checkdir-' . uniqid('', true);
        mkdir($this->projectDir, 0755, true);
        // checkIfPhpized() reads ext/Makefile relative to the CWD.
        chdir($this->projectDir);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);
        $this->removeRecursively($this->projectDir);
    }

    public function testDoesNotWarnOnSameVersionRebuild(): void
    {
        // The version cache dir already exists AND the project was phpized before.
        mkdir($this->projectDir . '/.zephir/' . Zephir::VERSION, 0755, true);
        $this->markPhpized();
        [$compiler, $logger] = $this->createCompiler();

        $this->invokeCheckDirectory($compiler);

        $this->assertFalse(
            $logger->hasInfoThatContains('version'),
            'No version-change warning must be logged when rebuilding the same version.'
        );
    }

    public function testWarnsWithVersionsOnGenuineChange(): void
    {
        // A different (older) version cache dir exists, the current one does not,
        // and the project was phpized before => genuine version change.
        mkdir($this->projectDir . '/.zephir/0.0.1-old', 0755, true);
        $this->markPhpized();
        [$compiler, $logger] = $this->createCompiler();

        $this->invokeCheckDirectory($compiler);

        $this->assertTrue($logger->hasInfoThatContains('Zephir version changed'));
        $this->assertTrue($logger->hasInfoThatContains('0.0.1-old'));
        $this->assertTrue($logger->hasInfoThatContains(Zephir::VERSION));
    }

    public function testDoesNotWarnOnFreshProject(): void
    {
        // No version cache dir and no ext/Makefile => first build ever.
        [$compiler, $logger] = $this->createCompiler();

        $this->invokeCheckDirectory($compiler);

        $this->assertFalse($logger->hasInfoThatContains('version'));
    }

    private function markPhpized(): void
    {
        mkdir($this->projectDir . '/ext', 0755, true);
        file_put_contents($this->projectDir . '/ext/Makefile', "# built\n");
    }

    private function invokeCheckDirectory(Compiler $compiler): void
    {
        $method = new ReflectionMethod(Compiler::class, 'checkDirectory');
        $method->setAccessible(true);
        $method->invoke($compiler);
    }

    /**
     * @return array{0: Compiler, 1: TestLogger}
     */
    private function createCompiler(): array
    {
        $logger  = new TestLogger();
        $config  = new Config();
        $config->set('namespace', 'test');
        $backend         = new Backend($config, 'kernel', 'templates');
        $disk            = new HardDisk($this->projectDir . '/.zephir');
        $compilerFactory = new CompilerFileFactory($config, $disk, $logger);

        $compiler = new Compiler($config, $backend, new Manager(new Parser()), $disk, $compilerFactory);
        $compiler->setLogger($logger);

        return [$compiler, $logger];
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
