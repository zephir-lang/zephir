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

namespace Zephir\Test\CodeGen;

use FilesystemIterator;
use Psr\Log\NullLogger;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use Zephir\Backend\Backend;
use Zephir\Backend\StringsManager;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Compiles a `.zep` source in a throwaway project and hands back the generated C.
 *
 * Every test in this directory needs the same thing: a temp project, a
 * `Compiler` wired to it, one source compiled, and the emitted text read back
 * so it can be asserted on. That harness was pasted into each of them, which is
 * how `tests/Zephir/CodeGen/` came to hold the largest block of duplicated code
 * in the project.
 *
 * A trait rather than a base class, following the precedent set by
 * Zephir\Test\BlackBox\RunsZephirCommands: nothing in `tests/` subclasses
 * anything, and this keeps every consumer a `final class ... extends TestCase`.
 *
 * The ordering inside setUpCodeGen() is load-bearing. `ZEPHIRPATH` is the
 * relative `'./'`, and `Config` reads `config.json` from the working directory,
 * so the chdir() has to happen after the file is written and before either is
 * used.
 */
trait CompilesZephirSource
{
    private Compiler            $compiler;
    private CompilerFileFactory $compilerFileFactory;
    private string              $originalCwd;
    private string              $tempDir;

    /**
     * Extracts one `PHP_METHOD(...) { ... }` from generated C, braces balanced.
     *
     * Returns an empty string when the signature is absent, so a caller can say
     * which method it was looking for.
     */
    protected function methodBody(string $haystack, string $signature): string
    {
        $startPos = strpos($haystack, $signature);
        if (false === $startPos) {
            return '';
        }

        $braceStart = strpos($haystack, '{', $startPos);
        if (false === $braceStart) {
            return '';
        }

        $depth  = 1;
        $i      = $braceStart + 1;
        $length = strlen($haystack);
        while ($i < $length && $depth > 0) {
            if ('{' === $haystack[$i]) {
                ++$depth;
            } elseif ('}' === $haystack[$i]) {
                --$depth;
            }
            ++$i;
        }

        return substr($haystack, $startPos, $i - $startPos);
    }

    /**
     * Writes a `.zep` source into the throwaway project and compiles it.
     *
     * `$relPath` is relative to the project root and keeps its `.zep` suffix,
     * e.g. `stub/issue2682/fetchdim.zep`, because that is what the compiler is
     * handed and what it derives the output path from.
     */
    protected function compileSource(string $fqcn, string $relPath, string $zep): void
    {
        file_put_contents($this->tempDir . '/' . $relPath, $zep);

        $compilerFile = $this->compilerFileFactory->create($fqcn, $relPath);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());
    }

    /**
     * The generated C for a source, addressed the way it was compiled:
     * `stub/issue2682/fetchdim.zep` reads `ext/stub/issue2682/fetchdim.zep.c`.
     */
    protected function generatedC(string $relPath): string
    {
        return $this->generatedFile($relPath . '.c');
    }

    /**
     * The generated header, for the arg-info tests that assert on `.zep.h`.
     */
    protected function generatedHeader(string $relPath): string
    {
        return $this->generatedFile($relPath . '.h');
    }

    /**
     * @param array<int, string> $sourceDirs  project-relative, created under
     *                                        both the project root and `ext/`
     * @param array<string,mixed> $config     written verbatim to `config.json`
     */
    protected function setUpCodeGen(
        string $tempPrefix,
        array $sourceDirs,
        array $config = ['namespace' => 'stub']
    ): void {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();
        $this->tempDir     = sys_get_temp_dir() . '/' . $tempPrefix . uniqid('', true);

        foreach ($sourceDirs as $sourceDir) {
            mkdir($this->tempDir . '/ext/' . $sourceDir, 0755, true);
            mkdir($this->tempDir . '/' . $sourceDir, 0755, true);
        }

        file_put_contents(
            $this->tempDir . '/config.json',
            json_encode($config, JSON_PRETTY_PRINT)
        );

        chdir($this->tempDir);

        $zephirConfig = new Config();
        $disk         = new HardDisk($this->tempDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');
        $logger  = new NullLogger();
        $backend = new Backend($zephirConfig, ZEPHIRPATH . 'kernel', ZEPHIRPATH . 'templates');

        $this->compilerFileFactory = new CompilerFileFactory($zephirConfig, $disk, $logger);

        $this->compiler = new Compiler(
            $zephirConfig,
            $backend,
            new Manager(new Parser()),
            $disk,
            $this->compilerFileFactory
        );
        $this->compiler->setLogger($logger);
    }

    protected function tearDownCodeGen(): void
    {
        chdir($this->originalCwd);

        if (!is_dir($this->tempDir)) {
            return;
        }

        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($this->tempDir, FilesystemIterator::SKIP_DOTS),
            RecursiveIteratorIterator::CHILD_FIRST
        );
        foreach ($iterator as $file) {
            $file->isDir() ? rmdir($file->getRealPath()) : unlink($file->getRealPath());
        }

        rmdir($this->tempDir);
    }

    private function generatedFile(string $relPath): string
    {
        $path = $this->tempDir . '/ext/' . $relPath;

        $this->assertFileExists($path, "The compiler should have emitted ext/$relPath.");

        return file_get_contents($path);
    }
}
