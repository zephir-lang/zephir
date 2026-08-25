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
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Covers how many translation units are compiled at once. Builds used to be
 * pinned to two jobs regardless of how many processors sat idle, and the C
 * compilation of an extension is entirely CPU bound.
 */
final class ParallelJobsTest extends TestCase
{
    private string $originalCwd;
    private string $projectDir;

    protected function setUp(): void
    {
        $this->originalCwd = getcwd();
        $this->projectDir  = sys_get_temp_dir() . '/zephir-jobs-' . uniqid('', true);
        mkdir($this->projectDir, 0755, true);
        chdir($this->projectDir);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);
        $this->removeRecursively($this->projectDir);
    }

    /**
     * @dataProvider processorCountProvider
     */
    public function testProcessorCountIsReadOffCommandOutput(string $raw, int $expected): void
    {
        $this->assertSame($expected, Compiler::parseProcessorCount($raw));
    }

    public static function processorCountProvider(): array
    {
        return [
            'nproc'             => ["4\n", 4],
            'no trailing newline' => ['4', 4],
            'padded'            => ['  12  ', 12],
            'two digits'        => ["64\n", 64],
            'nothing captured'  => ['', 0],
            'not a number'      => ["nproc: command not found\n", 0],
            'zero'              => ['0', 0],
        ];
    }

    public function testDefaultsToOneJobPerProcessor(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('The processor count comes from the environment on Windows.');
        }

        $expected = (int)trim((string)shell_exec('nproc 2>/dev/null'));
        if ($expected < 1) {
            $this->markTestSkipped('This machine does not report a processor count.');
        }

        [$compiler] = $this->createCompiler();

        $this->assertSame($expected, $this->resolveJobs($compiler, null));
    }

    public function testExplicitJobsWins(): void
    {
        [$compiler] = $this->createCompiler();

        $this->assertSame(1, $this->resolveJobs($compiler, 1));
        $this->assertSame(64, $this->resolveJobs($compiler, 64));
    }

    /**
     * `--jobs=0` and `--jobs=abc` both reach here as a non-positive number, and
     * `make -j0` is an error, so they have to fall back rather than pass through.
     */
    public function testNonPositiveJobsFallsBackToDetection(): void
    {
        [$compiler] = $this->createCompiler();

        $this->assertGreaterThan(0, $this->resolveJobs($compiler, 0));
        $this->assertGreaterThan(0, $this->resolveJobs($compiler, -4));
    }

    /**
     * Resolving is pure: compile() and install() each report the count they
     * ended up using, so resolving twice must not narrate anything.
     */
    public function testResolvingDoesNotLog(): void
    {
        [$compiler, $logger] = $this->createCompiler();

        $this->resolveJobs($compiler, null);
        $this->resolveJobs($compiler, 3);

        $this->assertFalse($logger->hasInfoThatContains('job'));
    }

    /**
     * install() runs `make` too, and `zephir install` on its own reaches it
     * with nothing built, which makes it a full build.
     */
    public function testInstallTakesAJobCount(): void
    {
        $install = new ReflectionMethod(Compiler::class, 'install');

        $this->assertSame(2, $install->getNumberOfParameters());
        $this->assertSame('jobs', $install->getParameters()[1]->getName());
        $this->assertTrue($install->getParameters()[1]->allowsNull());
    }

    private function resolveJobs(Compiler $compiler, ?int $jobs): int
    {
        $method = new ReflectionMethod(Compiler::class, 'resolveJobs');
        $method->setAccessible(true);

        return $method->invoke($compiler, $jobs);
    }

    /**
     * @return array{0: Compiler, 1: TestLogger}
     */
    private function createCompiler(): array
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
