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
 * Integration tests that compile full .zep files and compare generated C code.
 *
 * These tests validate the complete compilation pipeline from Zephir source
 * to C code generation.
 *
 * @group integration
 * @group slow
 */
final class FullCodeGenerationTest extends TestCase
{
    private string $fixturesPath;
    private HardDisk $filesystem;
    private Config $config;

    protected function setUp(): void
    {
        parent::setUp();
        $this->fixturesPath = FIXTURES_PATH . '/codegen';
        $this->filesystem = new HardDisk(ZEPHIRPATH);
        $this->config = new Config();
    }

    public function testSimpleClassGeneratesCorrectCCode(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class SimpleClass
{
    public function getValue() -> int
    {
        return 42;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'SimpleClass');

        // Verify class initialization
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(TestFixture_SimpleClass)', $generatedC);

        // Verify method declaration
        $this->assertStringContainsString('PHP_METHOD(TestFixture_SimpleClass, getValue)', $generatedC);

        // Verify return statement
        $this->assertStringContainsString('RETURN_LONG(42)', $generatedC);

        // Verify method entry table
        $this->assertStringContainsString('testfixture_simpleclass_method_entry', $generatedC);
    }

    public function testMethodWithParametersGeneratesCorrectCode(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class Calculator
{
    public function add(int a, int b) -> int
    {
        return a + b;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'Calculator');

        // Verify parameter parsing
        $this->assertStringContainsString('ZEND_PARSE_PARAMETERS_START(2, 2)', $generatedC);
        $this->assertStringContainsString('Z_PARAM_LONG(a)', $generatedC);
        $this->assertStringContainsString('Z_PARAM_LONG(b)', $generatedC);
        $this->assertStringContainsString('ZEND_PARSE_PARAMETERS_END()', $generatedC);

        // Verify variable declarations
        $this->assertMatchesRegularExpression('/zend_long\s+a\s*,\s*b/', $generatedC);
    }

    public function testVariableDeclarationsGenerateCorrectCode(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class Variables
{
    public function testVariables()
    {
        int count = 10;
        string name = "test";
        bool flag = true;
        double ratio = 1.5;

        return count;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'Variables');

        // Verify variable declarations
        $this->assertStringContainsString('zend_long count = 0', $generatedC);
        $this->assertStringContainsString('zend_bool flag = 0', $generatedC);
        $this->assertStringContainsString('double ratio = 0', $generatedC);

        // Verify initializations
        $this->assertStringContainsString('ZVAL_STRING', $generatedC);
        $this->assertStringContainsString('count = 10', $generatedC);
        $this->assertStringContainsString('flag = 1', $generatedC);
        $this->assertStringContainsString('ratio = 1.5', $generatedC);
    }

    public function testClassHeaderGeneratesCorrectStructure(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class HeaderTest
{
    public function methodOne() {}

    protected function methodTwo() {}

    public static function methodThree() {}
}
ZEP;

        $generatedH = $this->compileZephirToH($zepContent, 'HeaderTest');

        // Verify extern class entry
        $this->assertStringContainsString('extern zend_class_entry *testfixture_headertest_ce;', $generatedH);

        // Verify ZEPHIR_INIT_CLASS declaration
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(TestFixture_HeaderTest);', $generatedH);

        // Verify method forward declarations
        $this->assertStringContainsString('PHP_METHOD(TestFixture_HeaderTest, methodOne);', $generatedH);
        $this->assertStringContainsString('PHP_METHOD(TestFixture_HeaderTest, methodTwo);', $generatedH);
        $this->assertStringContainsString('PHP_METHOD(TestFixture_HeaderTest, methodThree);', $generatedH);

        // Verify method entry structure
        $this->assertStringContainsString('ZEPHIR_INIT_FUNCS(testfixture_headertest_method_entry)', $generatedH);
    }

    public function testCompleteMethodBodyStructure(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class MethodBody
{
    public function calculate(int x) -> int
    {
        int result;
        let result = x * 2;
        return result;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'MethodBody');

        // Verify complete structure in order
        $this->assertCodeStructureInOrder($generatedC, [
            'PHP_METHOD(TestFixture_MethodBody, calculate)',
            '{',
            'zend_long x',
            'zend_long result',
            'ZEND_PARSE_PARAMETERS_START',
            'Z_PARAM_LONG(x)',
            'ZEND_PARSE_PARAMETERS_END',
            'result = (x * 2)',
            'RETURN_LONG(result)',
            '}',
        ]);
    }

    public function testStringEscapingInGeneratedCode(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class StringTest
{
    public function getString() -> string
    {
        return "Hello \"World\"";
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'StringTest');

        // Verify string escaping
        $this->assertStringContainsString('RETURN_MM_STRING("Hello \\"World\\"")', $generatedC);
    }

    public function testArrayInitializationCode(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class ArrayTest
{
    public function getArray() -> array
    {
        var arr;
        let arr = [];
        return arr;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'ArrayTest');

        // Verify array initialization
        $this->assertStringContainsString('array_init', $generatedC);
    }

    public function testClassWithConstructor(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class WithConstructor
{
    protected value;

    public function __construct(int val)
    {
        let this->value = val;
    }

    public function getValue() -> int
    {
        return this->value;
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'WithConstructor');

        // Verify constructor
        $this->assertStringContainsString('PHP_METHOD(TestFixture_WithConstructor, __construct)', $generatedC);

        // Verify property update
        $this->assertStringContainsString('zephir_update_property_this', $generatedC);

        // Verify property read
        $this->assertStringContainsString('zephir_read_property', $generatedC);
    }

    public function testMethodWithMultipleReturns(): void
    {
        $zepContent = <<<'ZEP'
namespace TestFixture;

class ConditionalReturn
{
    public function check(int x) -> string
    {
        if x > 10 {
            return "large";
        } else {
            return "small";
        }
    }
}
ZEP;

        $generatedC = $this->compileZephirToC($zepContent, 'ConditionalReturn');

        // Verify conditional structure
        $this->assertStringContainsString('if', $generatedC);

        // Verify multiple return statements
        $this->assertGreaterThanOrEqual(2, substr_count($generatedC, 'RETURN_MM_STRING'));
    }

    /**
     * Helper method to compile Zephir code to C.
     */
    private function compileZephirToC(string $zepContent, string $className): string
    {
        $tempDir = sys_get_temp_dir() . '/zephir_test_' . uniqid();
        mkdir($tempDir);
        mkdir($tempDir . '/testfixture');

        try {
            // Write .zep file
            $zepFile = $tempDir . '/testfixture/' . strtolower($className) . '.zep';
            file_put_contents($zepFile, $zepContent);

            // Create minimal config.json
            $configFile = $tempDir . '/config.json';
            file_put_contents($configFile, json_encode([
                'namespace' => 'testfixture',
                'name' => 'Test Fixture',
                'author' => 'Test',
                'version' => '1.0.0',
            ]));

            // Compile
            $compiler = new Compiler($this->config, new Backend($this->config, '', ''), new Manager());
            $compiler->setFilesystem($this->filesystem);

            // Change to temp directory for compilation
            $originalDir = getcwd();
            chdir($tempDir);

            $compiler->generate();

            // Read generated C file
            $cFile = $tempDir . '/ext/testfixture/' . strtolower($className) . '.zep.c';

            chdir($originalDir);

            if (!file_exists($cFile)) {
                $this->fail("Generated C file not found: {$cFile}");
            }

            return file_get_contents($cFile);
        } finally {
            // Cleanup
            if (isset($originalDir)) {
                chdir($originalDir);
            }
            $this->recursiveRemoveDirectory($tempDir);
        }
    }

    /**
     * Helper method to compile Zephir code to header file.
     */
    private function compileZephirToH(string $zepContent, string $className): string
    {
        $tempDir = sys_get_temp_dir() . '/zephir_test_' . uniqid();
        mkdir($tempDir);
        mkdir($tempDir . '/testfixture');

        try {
            // Write .zep file
            $zepFile = $tempDir . '/testfixture/' . strtolower($className) . '.zep';
            file_put_contents($zepFile, $zepContent);

            // Create minimal config.json
            $configFile = $tempDir . '/config.json';
            file_put_contents($configFile, json_encode([
                'namespace' => 'testfixture',
                'name' => 'Test Fixture',
                'author' => 'Test',
                'version' => '1.0.0',
            ]));

            // Compile
            $compiler = new Compiler($this->config, new Backend($this->config, '', ''), new Manager());
            $compiler->setFilesystem($this->filesystem);

            // Change to temp directory for compilation
            $originalDir = getcwd();
            chdir($tempDir);

            $compiler->generate();

            // Read generated header file
            $hFile = $tempDir . '/ext/testfixture/' . strtolower($className) . '.zep.h';

            chdir($originalDir);

            if (!file_exists($hFile)) {
                $this->fail("Generated header file not found: {$hFile}");
            }

            return file_get_contents($hFile);
        } finally {
            // Cleanup
            if (isset($originalDir)) {
                chdir($originalDir);
            }
            $this->recursiveRemoveDirectory($tempDir);
        }
    }

    /**
     * Assert that code contains strings in the specified order.
     */
    private function assertCodeStructureInOrder(string $code, array $needles): void
    {
        $lastPos = -1;

        foreach ($needles as $needle) {
            $pos = strpos($code, $needle);
            $this->assertNotFalse($pos, "Could not find: {$needle}");
            $this->assertGreaterThan(
                $lastPos,
                $pos,
                "Order violation for: {$needle}. Expected after position {$lastPos}, found at {$pos}"
            );
            $lastPos = $pos;
        }
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
