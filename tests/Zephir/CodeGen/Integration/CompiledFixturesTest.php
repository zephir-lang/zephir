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
use Zephir\CompilationContext;
use Zephir\Compiler;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;

/**
 * Tests that validate generated C code matches existing compiled fixtures.
 *
 * These tests pick actual .zep files from stub/, compile them, and compare
 * the generated C/H files against the existing .zep.c and .zep.h files.
 * This ensures code generation consistency and catches regressions.
 *
 * @group integration
 */
final class CompiledFixturesTest extends TestCase
{
    private string $stubPath;
    private string $extStubPath;
    private Config $config;
    private HardDisk $filesystem;

    protected function setUp(): void
    {
        parent::setUp();
        $this->stubPath = ZEPHIRPATH . '/stub';
        $this->extStubPath = ZEPHIRPATH . '/ext/stub';
        $this->config = new Config();
        $this->filesystem = new HardDisk(ZEPHIRPATH);
    }

    /**
     * Test simple arithmetic operations class.
     */
    public function testArithmeticClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('arithmetic');
    }

    /**
     * Test existence checking class.
     */
    public function testExistsClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('exists');
    }

    /**
     * Test factorial calculation class.
     */
    public function testFactorialClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('factorial');
    }

    /**
     * Test method call class.
     */
    public function testMcallClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('mcall');
    }

    /**
     * Test internal method call class.
     */
    public function testMcallInternalClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('mcallinternal');
    }

    /**
     * Test logical operations class.
     */
    public function testLogicalClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('logical');
    }

    /**
     * Test exception handling class.
     */
    public function testExceptionsClassGeneration(): void
    {
        $this->assertCompiledFixtureMatches('exceptions');
    }

    /**
     * Test interface definition.
     */
    public function testMethodInterfaceGeneration(): void
    {
        $this->assertCompiledFixtureMatches('methodinterface');
    }

    /**
     * Test closure generation.
     */
    public function testClosureGeneration(): void
    {
        $this->assertCompiledFixtureMatches('0__closure');
    }

    /**
     * Test PDO statement class.
     */
    public function testPdoStatementGeneration(): void
    {
        $this->assertCompiledFixtureMatches('pdostatement');
    }

    /**
     * Assert that recompiling a fixture generates identical code to existing compiled version.
     */
    private function assertCompiledFixtureMatches(string $fixtureName): void
    {
        $zepFile = $this->stubPath . '/' . $fixtureName . '.zep';
        $existingCFile = $this->extStubPath . '/' . $fixtureName . '.zep.c';
        $existingHFile = $this->extStubPath . '/' . $fixtureName . '.zep.h';

        // Skip if fixture doesn't exist
        if (!file_exists($zepFile)) {
            $this->markTestSkipped("Fixture not found: {$zepFile}");
        }

        if (!file_exists($existingCFile)) {
            $this->markTestSkipped("Compiled C file not found: {$existingCFile}");
        }

        if (!file_exists($existingHFile)) {
            $this->markTestSkipped("Compiled H file not found: {$existingHFile}");
        }

        // Read existing compiled files
        $existingC = file_get_contents($existingCFile);
        $existingH = file_get_contents($existingHFile);

        // Generate new C/H code from the .zep file
        [$generatedC, $generatedH] = $this->recompileFixture($zepFile, $fixtureName);

        // Normalize for comparison (remove comments, normalize whitespace)
        $existingC = $this->normalizeCode($existingC);
        $existingH = $this->normalizeCode($existingH);
        $generatedC = $this->normalizeCode($generatedC);
        $generatedH = $this->normalizeCode($generatedH);

        // Compare C file
        if ($existingC !== $generatedC) {
            $this->createDiffFile($fixtureName . '.zep.c', $existingC, $generatedC);
            $this->assertEquals(
                $existingC,
                $generatedC,
                "Generated C code for {$fixtureName} differs from existing compiled version.\n" .
                "Diff saved to: /tmp/zephir_codegen_diff_{$fixtureName}.c.diff\n" .
                "This could indicate a regression in code generation."
            );
        }

        // Compare H file
        if ($existingH !== $generatedH) {
            $this->createDiffFile($fixtureName . '.zep.h', $existingH, $generatedH);
            $this->assertEquals(
                $existingH,
                $generatedH,
                "Generated H code for {$fixtureName} differs from existing compiled version.\n" .
                "Diff saved to: /tmp/zephir_codegen_diff_{$fixtureName}.h.diff\n" .
                "This could indicate a regression in code generation."
            );
        }

        // If we get here, both files match
        $this->addToAssertionCount(2); // Count both C and H comparisons
    }

    /**
     * Recompile a .zep fixture file and return generated C and H code.
     */
    private function recompileFixture(string $zepFile, string $fixtureName): array
    {
        // Parse the .zep file to get IR
        $parser = (new Manager())->getParser();
        $ir = $parser->parse($zepFile);

        if (!isset($ir['type']) || $ir['type'] === 'error') {
            $this->fail("Failed to parse {$zepFile}: " . ($ir['message'] ?? 'Unknown error'));
        }

        // Create compiler file
        $compilerFile = new CompilerFile($this->config, new \Zephir\AliasManager(), $this->filesystem);

        // Extract class name from IR
        $className = null;
        foreach ($ir as $statement) {
            if (isset($statement['type']) && in_array($statement['type'], ['class', 'interface'])) {
                $className = $statement['name'];
                break;
            }
        }

        if (!$className) {
            $this->fail("Could not determine class name from {$zepFile}");
        }

        $compilerFile->setClassName('Stub\\' . $className);
        $compilerFile->setFilePath($zepFile);

        // Create compiler and backend
        $compiler = new Compiler($this->config, new Backend($this->config, '', ''), new Manager());
        $compiler->setFilesystem($this->filesystem);

        // Pre-compile to get class definition
        $compilerFile->preCompile($compiler);

        // Create strings manager
        $stringsManager = new \Zephir\StringsManager();

        // Compile the file
        $compilerFile->compile($compiler, $stringsManager);

        // Get the generated code
        $compiledFile = $compilerFile->getCompiledFile();

        if (!$compiledFile) {
            $this->fail("No compiled file generated for {$zepFile}");
        }

        // Read generated files from the temp location or directly from compilation result
        // The compilation should have generated the C and H files
        $cContent = file_exists($compiledFile) ? file_get_contents($compiledFile) : '';
        $hFile = str_replace('.zep.c', '.zep.h', $compiledFile);
        $hContent = file_exists($hFile) ? file_get_contents($hFile) : '';

        return [$cContent, $hContent];
    }

    /**
     * Normalize generated code for comparison.
     */
    private function normalizeCode(string $code): string
    {
        // Remove C-style comments
        $code = preg_replace('#/\*.*?\*/#s', '', $code);

        // Remove C++ style comments
        $code = preg_replace('#//.*$#m', '', $code);

        // Normalize line endings
        $code = str_replace("\r\n", "\n", $code);

        // Remove trailing whitespace from each line
        $lines = explode("\n", $code);
        $lines = array_map('rtrim', $lines);

        // Remove empty lines at start and end
        while (count($lines) > 0 && trim($lines[0]) === '') {
            array_shift($lines);
        }
        while (count($lines) > 0 && trim($lines[count($lines) - 1]) === '') {
            array_pop($lines);
        }

        return implode("\n", $lines);
    }

    /**
     * Create a diff file for debugging.
     */
    private function createDiffFile(string $filename, string $expected, string $actual): void
    {
        $diffFile = '/tmp/zephir_codegen_diff_' . basename($filename) . '.diff';

        $expectedLines = explode("\n", $expected);
        $actualLines = explode("\n", $actual);

        $diff = "=== DIFF for {$filename} ===\n";
        $diff .= "Lines: Expected=" . count($expectedLines) . ", Actual=" . count($actualLines) . "\n\n";

        $maxLines = max(count($expectedLines), count($actualLines));
        $diffCount = 0;

        for ($i = 0; $i < $maxLines && $diffCount < 100; $i++) {
            $expLine = $expectedLines[$i] ?? '<missing>';
            $actLine = $actualLines[$i] ?? '<missing>';

            if ($expLine !== $actLine) {
                $diff .= "Line " . ($i + 1) . ":\n";
                $diff .= "  - Expected: " . substr($expLine, 0, 200) . "\n";
                $diff .= "  + Actual:   " . substr($actLine, 0, 200) . "\n\n";
                $diffCount++;
            }
        }

        if ($maxLines > 100) {
            $diff .= "\n... (output truncated, showing first 100 differences)\n";
        }

        file_put_contents($diffFile, $diff);
    }
}
