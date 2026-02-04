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

namespace Zephir\Test\CodeGen\Integration;

use PHPUnit\Framework\TestCase;
use Zephir\Backend\Backend;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;

/**
 * Snapshot tests that compare generated C code against expected output.
 *
 * These tests use "golden files" - pre-generated expected output that is
 * version controlled. When code generation changes, snapshots can be updated
 * to reflect the new expected output.
 *
 * @group integration
 * @group slow
 */
final class SnapshotTest extends TestCase
{
    private string $fixturesPath;
    private string $snapshotsPath;
    private HardDisk $filesystem;
    private Config $config;
    private bool $updateSnapshots = false;

    protected function setUp(): void
    {
        parent::setUp();
        $this->fixturesPath = FIXTURES_PATH . '/codegen';
        $this->snapshotsPath = __DIR__ . '/snapshots';
        $this->filesystem = new HardDisk(ZEPHIRPATH);
        $this->config = new Config();

        // Set to true to update snapshots when code generation changes
        // Usage: UPDATE_SNAPSHOTS=1 phpunit tests/Zephir/CodeGen/Integration/SnapshotTest.php
        $this->updateSnapshots = !empty(getenv('UPDATE_SNAPSHOTS'));

        if (!is_dir($this->snapshotsPath)) {
            mkdir($this->snapshotsPath, 0755, true);
        }
    }

    public function testSimpleClassSnapshot(): void
    {
        $zepFile = $this->fixturesPath . '/simple_class.zep';
        $this->assertFileExists($zepFile, "Fixture file not found");

        $generatedC = $this->compileFixtureToC('simple_class');
        $this->assertSnapshot('simple_class.c', $generatedC);
    }

    public function testTypedParamsSnapshot(): void
    {
        $zepFile = $this->fixturesPath . '/typed_params.zep';
        $this->assertFileExists($zepFile, "Fixture file not found");

        $generatedC = $this->compileFixtureToC('typed_params');
        $this->assertSnapshot('typed_params.c', $generatedC);
    }

    public function testVariableTypesSnapshot(): void
    {
        $zepFile = $this->fixturesPath . '/variable_types.zep';
        $this->assertFileExists($zepFile, "Fixture file not found");

        $generatedC = $this->compileFixtureToC('variable_types');
        $this->assertSnapshot('variable_types.c', $generatedC);
    }

    public function testSimpleClassHeaderSnapshot(): void
    {
        $zepFile = $this->fixturesPath . '/simple_class.zep';
        $this->assertFileExists($zepFile, "Fixture file not found");

        $generatedH = $this->compileFixtureToH('simple_class');
        $this->assertSnapshot('simple_class.h', $generatedH);
    }

    /**
     * Assert generated code matches snapshot or update snapshot if requested.
     */
    private function assertSnapshot(string $snapshotName, string $generatedCode): void
    {
        $snapshotFile = $this->snapshotsPath . '/' . $snapshotName;

        if ($this->updateSnapshots) {
            // Update mode: write new snapshot
            file_put_contents($snapshotFile, $generatedCode);
            $this->markTestIncomplete("Updated snapshot: {$snapshotName}");
            return;
        }

        if (!file_exists($snapshotFile)) {
            // No snapshot exists yet - create it
            file_put_contents($snapshotFile, $generatedCode);
            $this->markTestIncomplete(
                "Created initial snapshot: {$snapshotName}. " .
                "Run tests again to validate."
            );
            return;
        }

        // Compare with existing snapshot
        $expected = file_get_contents($snapshotFile);

        if ($expected !== $generatedCode) {
            // Create a diff file to help debugging
            $diffFile = $this->snapshotsPath . '/' . $snapshotName . '.diff';
            file_put_contents($diffFile, $this->createDiff($expected, $generatedCode));

            $this->assertEquals(
                $expected,
                $generatedCode,
                "Generated code differs from snapshot: {$snapshotName}\n" .
                "Diff saved to: {$diffFile}\n" .
                "To update snapshots, run: UPDATE_SNAPSHOTS=1 phpunit " . static::class
            );
        }
    }

    /**
     * Create a simple diff between expected and actual.
     */
    private function createDiff(string $expected, string $actual): string
    {
        $expectedLines = explode("\n", $expected);
        $actualLines = explode("\n", $actual);

        $diff = "=== DIFF ===\n";
        $diff .= "Lines: Expected=" . count($expectedLines) . ", Actual=" . count($actualLines) . "\n\n";

        $maxLines = max(count($expectedLines), count($actualLines));
        for ($i = 0; $i < min($maxLines, 50); $i++) {
            $expLine = $expectedLines[$i] ?? '';
            $actLine = $actualLines[$i] ?? '';

            if ($expLine !== $actLine) {
                $diff .= "Line " . ($i + 1) . ":\n";
                $diff .= "  - Expected: " . substr($expLine, 0, 100) . "\n";
                $diff .= "  + Actual:   " . substr($actLine, 0, 100) . "\n";
            }
        }

        if ($maxLines > 50) {
            $diff .= "\n... (" . ($maxLines - 50) . " more lines)\n";
        }

        return $diff;
    }

    /**
     * Compile a fixture .zep file to C code.
     */
    private function compileFixtureToC(string $fixtureName): string
    {
        $zepFile = $this->fixturesPath . '/' . $fixtureName . '.zep';

        if (!file_exists($zepFile)) {
            $this->fail("Fixture not found: {$zepFile}");
        }

        $tempDir = sys_get_temp_dir() . '/zephir_snapshot_' . uniqid();
        mkdir($tempDir);
        mkdir($tempDir . '/testfixture');

        try {
            // Copy fixture to temp directory
            copy($zepFile, $tempDir . '/testfixture/' . $fixtureName . '.zep');

            // Create config.json
            file_put_contents($tempDir . '/config.json', json_encode([
                'namespace' => 'testfixture',
                'name' => 'TestFixture',
                'author' => 'Test',
                'version' => '1.0.0',
            ]));

            // Compile
            $compiler = new Compiler($this->config, new Backend($this->config, '', ''), new Manager());
            $compiler->setFilesystem($this->filesystem);

            $originalDir = getcwd();
            chdir($tempDir);

            $compiler->generate();

            // Read generated C file
            $cFile = $tempDir . '/ext/testfixture/' . $fixtureName . '.zep.c';

            chdir($originalDir);

            if (!file_exists($cFile)) {
                $this->fail("Generated C file not found: {$cFile}");
            }

            // Normalize the output for consistent comparison
            $content = file_get_contents($cFile);
            return $this->normalizeGeneratedCode($content);
        } finally {
            if (isset($originalDir)) {
                chdir($originalDir);
            }
            $this->recursiveRemoveDirectory($tempDir);
        }
    }

    /**
     * Compile a fixture .zep file to header code.
     */
    private function compileFixtureToH(string $fixtureName): string
    {
        $zepFile = $this->fixturesPath . '/' . $fixtureName . '.zep';

        if (!file_exists($zepFile)) {
            $this->fail("Fixture not found: {$zepFile}");
        }

        $tempDir = sys_get_temp_dir() . '/zephir_snapshot_' . uniqid();
        mkdir($tempDir);
        mkdir($tempDir . '/testfixture');

        try {
            // Copy fixture to temp directory
            copy($zepFile, $tempDir . '/testfixture/' . $fixtureName . '.zep');

            // Create config.json
            file_put_contents($tempDir . '/config.json', json_encode([
                'namespace' => 'testfixture',
                'name' => 'TestFixture',
                'author' => 'Test',
                'version' => '1.0.0',
            ]));

            // Compile
            $compiler = new Compiler($this->config, new Backend($this->config, '', ''), new Manager());
            $compiler->setFilesystem($this->filesystem);

            $originalDir = getcwd();
            chdir($tempDir);

            $compiler->generate();

            // Read generated header file
            $hFile = $tempDir . '/ext/testfixture/' . $fixtureName . '.zep.h';

            chdir($originalDir);

            if (!file_exists($hFile)) {
                $this->fail("Generated header file not found: {$hFile}");
            }

            // Normalize the output for consistent comparison
            $content = file_get_contents($hFile);
            return $this->normalizeGeneratedCode($content);
        } finally {
            if (isset($originalDir)) {
                chdir($originalDir);
            }
            $this->recursiveRemoveDirectory($tempDir);
        }
    }

    /**
     * Normalize generated code for consistent comparison.
     * This removes comments and standardizes whitespace.
     */
    private function normalizeGeneratedCode(string $code): string
    {
        // Remove single-line comments starting with //
        $code = preg_replace('#//.*$#m', '', $code);

        // Remove multi-line comments /* ... */
        $code = preg_replace('#/\*.*?\*/#s', '', $code);

        // Normalize line endings
        $code = str_replace("\r\n", "\n", $code);

        // Remove trailing whitespace from each line
        $lines = explode("\n", $code);
        $lines = array_map('rtrim', $lines);

        return implode("\n", $lines);
    }

    /**
     * Recursively remove a directory.
     */
    private function recursiveRemoveDirectory(string $directory): void
    {
        if (!is_dir($directory)) {
            return;
        }

        $files = array_diff(scandir($directory), ['.', '..']);
        foreach ($files as $file) {
            $path = $directory . '/' . $file;
            is_dir($path) ? $this->recursiveRemoveDirectory($path) : unlink($path);
        }
        rmdir($directory);
    }
}
