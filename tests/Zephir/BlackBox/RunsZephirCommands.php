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

trait RunsZephirCommands
{
    private string $originalCwd = '';

    /** @var list<string> */
    private array $pathsToCleanup = [];

    protected function setUpZephirRunner(): void
    {
        $this->originalCwd = getcwd();
    }

    protected function tearDownZephirRunner(): void
    {
        if ($this->originalCwd !== '' && getcwd() !== $this->originalCwd) {
            chdir($this->originalCwd);
        }

        foreach ($this->pathsToCleanup as $path) {
            $this->removePath($path);
        }
        $this->pathsToCleanup = [];
    }

    protected function projectRoot(): string
    {
        return realpath(__DIR__ . '/../../..');
    }

    protected function fixturesDir(): string
    {
        return $this->projectRoot() . '/tests/fixtures';
    }

    protected function outputDir(): string
    {
        return $this->projectRoot() . '/tests/output';
    }

    protected function zephirBin(): string
    {
        return $this->projectRoot() . '/zephir';
    }

    protected function phpBin(): string
    {
        return PHP_BINARY;
    }

    /**
     * Run `php zephir <args>` and return exit code, stdout, stderr.
     *
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    protected function runZephir(string $args, ?string $cwd = null): array
    {
        $command = sprintf(
            '%s %s %s',
            escapeshellarg($this->phpBin()),
            escapeshellarg($this->zephirBin()),
            $args,
        );

        $descriptors = [
            0 => ['pipe', 'r'],
            1 => ['pipe', 'w'],
            2 => ['pipe', 'w'],
        ];

        $process = proc_open($command, $descriptors, $pipes, $cwd ?: null);
        if (!is_resource($process)) {
            return ['exitCode' => -1, 'stdout' => '', 'stderr' => 'Failed to start process'];
        }

        fclose($pipes[0]);
        $stdout = stream_get_contents($pipes[1]);
        $stderr = stream_get_contents($pipes[2]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        $exitCode = proc_close($process);

        return [
            'exitCode' => $exitCode,
            'stdout' => $stdout === false ? '' : $stdout,
            'stderr' => $stderr === false ? '' : $stderr,
        ];
    }

    /**
     * Mark a path for cleanup in tearDown.
     */
    protected function cleanupPath(string $path): void
    {
        $this->pathsToCleanup[] = $path;
    }

    protected function removePath(string $path): void
    {
        if (!file_exists($path) && !is_link($path)) {
            return;
        }

        if (is_link($path) || !is_dir($path)) {
            @unlink($path);
            return;
        }

        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($path, \FilesystemIterator::SKIP_DOTS),
            \RecursiveIteratorIterator::CHILD_FIRST,
        );

        foreach ($iterator as $entry) {
            /** @var \SplFileInfo $entry */
            if ($entry->isDir() && !$entry->isLink()) {
                @rmdir($entry->getPathname());
            } else {
                @unlink($entry->getPathname());
            }
        }

        @rmdir($path);
    }
}
