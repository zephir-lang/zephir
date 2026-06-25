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

namespace Zephir\Zept;

use function array_merge;
use function bin2hex;
use function file_put_contents;
use function getenv;
use function is_dir;
use function is_file;
use function is_resource;
use function json_encode;
use function ltrim;
use function mkdir;
use function preg_match;
use function proc_close;
use function proc_open;
use function random_bytes;
use function rtrim;
use function stream_get_contents;
use function strncmp;
use function sys_get_temp_dir;
use function trim;

/**
 * Runs a `.zept` case end-to-end, with no dependency on PHPUnit so it can back
 * the `zephir test` CLI command as well as the test-suite bridge.
 *
 * For each case it materialises a throwaway Zephir project, generates and
 * compiles the extension (forcing the pure-PHP parser, so every `.zept` doubles
 * as an end-to-end parser test), loads the freshly built `.so` into a clean PHP
 * subprocess, runs the `--USAGE--` script, and compares stdout to the
 * expectation. No `install` step is used, so no root or shared extension dir is
 * required.
 */
final class ZeptRunner
{
    private string $zephirBin;
    private string $phpBin;
    private string $workRoot;
    private OutputMatcher $matcher;

    public function __construct(
        string $zephirBin,
        ?string $phpBin = null,
        ?string $workRoot = null,
        ?OutputMatcher $matcher = null
    ) {
        $this->zephirBin = $zephirBin;
        $this->phpBin    = $phpBin ?? PHP_BINARY;
        $this->workRoot  = $workRoot ?? sys_get_temp_dir();
        $this->matcher   = $matcher ?? new OutputMatcher();
    }

    public function run(ZeptFile $zept): ZeptResult
    {
        try {
            $project   = new ZeptProject($zept);
            $namespace = $project->namespace();
            $sources   = $project->sources();
        } catch (ZeptParseException $e) {
            return ZeptResult::error($e->getMessage());
        }

        $skip = $this->checkSkip($zept);
        if ($skip !== null) {
            return ZeptResult::skip($skip);
        }

        $dir = $this->makeWorkDir();

        try {
            $this->writeProject($dir, $namespace, $sources);

            // Force the pure-PHP parser so the .zept also exercises it end-to-end.
            $env = array_merge($this->env(), ['ZEPHIR_FORCE_PHP_PARSER' => '1']);

            $generate = $this->runProcess([$this->phpBin, $this->zephirBin, 'generate'], $dir, $env);
            if ($generate['exitCode'] !== 0) {
                return ZeptResult::error("zephir generate failed:\n" . $generate['stderr'] . $generate['stdout']);
            }

            // `zephir compile` does not propagate a non-zero exit code, so the
            // produced .so is the reliable success signal.
            $compile = $this->runProcess([$this->phpBin, $this->zephirBin, 'compile'], $dir, $env);
            $so      = $dir . '/ext/modules/' . $namespace . '.so';
            if (!is_file($so)) {
                return ZeptResult::error(
                    "zephir compile produced no extension:\n" . $compile['stderr'] . $compile['stdout']
                );
            }

            $usageFile = $dir . '/usage.php';
            file_put_contents($usageFile, $this->asPhpScript($zept->usage));

            $command = [$this->phpBin, '-d', 'extension=' . $so];
            foreach ($this->iniDirectives($zept) as $directive) {
                $command[] = '-d';
                $command[] = $directive;
            }
            $command[] = $usageFile;

            $usage = $this->runProcess($command, $dir);

            if ($this->matcher->matches($zept->expect, $usage['stdout'], $zept->expectIsFormat)) {
                return ZeptResult::pass();
            }

            $reason = 'output does not match expectation';
            if (trim($usage['stderr']) !== '') {
                $reason .= ' (stderr: ' . trim($usage['stderr']) . ')';
            }

            return ZeptResult::fail($reason, $zept->expect, $usage['stdout']);
        } finally {
            $this->removeDir($dir);
        }
    }

    /**
     * @return ?string The skip reason, or null when the case must run.
     */
    private function checkSkip(ZeptFile $zept): ?string
    {
        if ($zept->skipif === null) {
            return null;
        }

        $dir  = $this->makeWorkDir();
        $file = $dir . '/skipif.php';

        try {
            file_put_contents($file, $this->asPhpScript($zept->skipif));
            $result = $this->runProcess([$this->phpBin, $file], $dir);
            $stdout = trim($result['stdout']);

            if (preg_match('/^skip\b\s*(.*)$/is', $stdout, $m) === 1) {
                return trim($m[1]) !== '' ? trim($m[1]) : 'skipped';
            }

            return null;
        } finally {
            $this->removeDir($dir);
        }
    }

    private function writeProject(string $dir, string $namespace, array $sources): void
    {
        file_put_contents(
            $dir . '/config.json',
            (string) json_encode(['namespace' => $namespace, 'name' => $namespace])
        );

        foreach ($sources as $relative => $source) {
            $full = $dir . '/' . $relative;
            $sub  = \dirname($full);
            if (!is_dir($sub)) {
                mkdir($sub, 0777, true);
            }
            file_put_contents($full, $source);
        }
    }

    /**
     * @return list<string>
     */
    private function iniDirectives(ZeptFile $zept): array
    {
        if ($zept->ini === null) {
            return [];
        }

        $directives = [];
        foreach (explode("\n", $zept->ini) as $line) {
            $line = trim($line);
            if ($line !== '' && strncmp($line, ';', 1) !== 0 && strpos($line, '=') !== false) {
                $directives[] = $line;
            }
        }

        return $directives;
    }

    private function asPhpScript(string $code): string
    {
        $trimmed = ltrim($code);
        if (strncmp($trimmed, '<?', 2) === 0) {
            return $code;
        }

        return "<?php\n" . $code;
    }

    /**
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function runProcess(array $command, string $cwd, ?array $env = null): array
    {
        $descriptors = [
            0 => ['pipe', 'r'],
            1 => ['pipe', 'w'],
            2 => ['pipe', 'w'],
        ];

        $process = proc_open($command, $descriptors, $pipes, $cwd, $env);
        if (!is_resource($process)) {
            return ['exitCode' => -1, 'stdout' => '', 'stderr' => 'failed to start process'];
        }

        fclose($pipes[0]);
        $stdout = stream_get_contents($pipes[1]);
        $stderr = stream_get_contents($pipes[2]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        $exitCode = proc_close($process);

        return [
            'exitCode' => $exitCode,
            'stdout'   => $stdout === false ? '' : $stdout,
            'stderr'   => $stderr === false ? '' : $stderr,
        ];
    }

    /**
     * @return array<string, string>
     */
    private function env(): array
    {
        $env = getenv();

        return is_array($env) ? $env : [];
    }

    private function makeWorkDir(): string
    {
        $dir = rtrim($this->workRoot, '/') . '/zept-' . bin2hex(random_bytes(6));
        mkdir($dir, 0777, true);

        return $dir;
    }

    private function removeDir(string $path): void
    {
        if (!is_dir($path)) {
            return;
        }

        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($path, \FilesystemIterator::SKIP_DOTS),
            \RecursiveIteratorIterator::CHILD_FIRST
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
