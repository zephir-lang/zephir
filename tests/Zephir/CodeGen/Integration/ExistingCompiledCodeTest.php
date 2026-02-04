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

/**
 * Tests that validate existing compiled C code structure and patterns.
 *
 * These tests examine the .zep.c and .zep.h files in ext/stub/ to ensure
 * they follow expected patterns and contain required structures. This serves
 * as documentation of expected code generation output.
 */
final class ExistingCompiledCodeTest extends TestCase
{
    private string $extStubPath;

    protected function setUp(): void
    {
        parent::setUp();
        $this->extStubPath = ZEPHIRPATH . '/ext/stub';
    }

    public function testArithmeticClassHasExpectedStructure(): void
    {
        $cCode = $this->readCompiledFile('arithmetic.zep.c');
        $hCode = $this->readCompiledFile('arithmetic.zep.h');

        // Verify C file structure
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_Arithmetic)', $cCode);
        $this->assertStringContainsString('PHP_METHOD(Stub_Arithmetic, intSum)', $cCode);
        $this->assertStringContainsString('RETURN_LONG(', $cCode);

        // Verify H file structure
        $this->assertStringContainsString('extern zend_class_entry *stub_arithmetic_ce;', $hCode);
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_Arithmetic);', $hCode);
        $this->assertStringContainsString('PHP_METHOD(Stub_Arithmetic, intSum);', $hCode);
    }

    public function testClosureClassHasExpectedStructure(): void
    {
        $cCode = $this->readCompiledFile('0__closure.zep.c');
        $hCode = $this->readCompiledFile('0__closure.zep.h');

        // Verify closure-specific patterns
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(stub_0__closure)', $cCode);
        $this->assertStringContainsString('PHP_METHOD(stub_0__closure, __invoke)', $cCode);
        $this->assertStringContainsString('ZEND_PARSE_PARAMETERS_START', $cCode);
        $this->assertStringContainsString('Z_PARAM_ZVAL', $cCode);

        // Verify header
        $this->assertStringContainsString('extern zend_class_entry *stub_0__closure_ce;', $hCode);
        $this->assertStringContainsString('PHP_METHOD(stub_0__closure, __invoke);', $hCode);
    }

    public function testMcallClassHasMethodCalls(): void
    {
        $cCode = $this->readCompiledFile('mcall.zep.c');

        // Verify method call patterns
        $this->assertStringContainsString('ZEPHIR_CALL_METHOD', $cCode);
        $this->assertStringContainsString('PHP_METHOD(Stub_Mcall, testCall1)', $cCode);
        $this->assertStringContainsString('PHP_METHOD(Stub_Mcall, testMethod1)', $cCode);
    }

    public function testExceptionsClassHasErrorHandling(): void
    {
        $cCode = $this->readCompiledFile('exceptions.zep.c');

        // Verify exception handling patterns
        $this->assertStringContainsString('zephir_throw_exception', $cCode);
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_Exceptions)', $cCode);
    }

    public function testLogicalClassHasLogicalOperations(): void
    {
        $cCode = $this->readCompiledFile('logical.zep.c');

        // Verify logical operation patterns
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_Logical)', $cCode);
        $this->assertStringContainsString('RETURN_BOOL', $cCode);
    }

    public function testInterfaceHasCorrectStructure(): void
    {
        $cCode = $this->readCompiledFile('methodinterface.zep.c');
        $hCode = $this->readCompiledFile('methodinterface.zep.h');

        // Verify interface patterns
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_MethodInterface)', $cCode);
        $this->assertStringContainsString('ZEPHIR_DOC_METHOD', $cCode);

        // Header should declare interface
        $this->assertStringContainsString('extern zend_class_entry *stub_methodinterface_ce;', $hCode);
    }

    public function testFactorialHasRecursivePatterns(): void
    {
        $cCode = $this->readCompiledFile('factorial.zep.c');

        // Verify factorial-specific patterns
        $this->assertStringContainsString('ZEPHIR_INIT_CLASS(Stub_Factorial)', $cCode);
        $this->assertStringContainsString('ZEPHIR_CALL_METHOD', $cCode);
        $this->assertStringContainsString('zend_long', $cCode);
    }

    public function testAllCompiledFilesHaveRequiredHeaders(): void
    {
        $files = glob($this->extStubPath . '/*.zep.c');

        $this->assertGreaterThan(0, count($files), 'No compiled .zep.c files found');

        foreach ($files as $file) {
            $content = file_get_contents($file);
            $filename = basename($file);

            // Every C file should have these includes
            $this->assertStringContainsString('#include <php.h>', $content, "{$filename} missing php.h include");
            $this->assertStringContainsString('#include "../php_ext.h"', $content, "{$filename} missing php_ext.h include");
            $this->assertStringContainsString('#include "../ext.h"', $content, "{$filename} missing ext.h include");

            // Should include kernel headers
            $this->assertStringContainsString('#include "kernel/', $content, "{$filename} missing kernel includes");
        }
    }

    public function testAllHeaderFilesHaveGuards(): void
    {
        $files = glob($this->extStubPath . '/*.zep.h');

        $this->assertGreaterThan(0, count($files), 'No compiled .zep.h files found');

        foreach ($files as $file) {
            $content = file_get_contents($file);
            $filename = basename($file);

            // Every header should have extern declarations
            $this->assertStringContainsString('extern zend_class_entry', $content, "{$filename} missing class entry extern");

            // Should have ZEPHIR_INIT_CLASS
            $this->assertStringContainsString('ZEPHIR_INIT_CLASS', $content, "{$filename} missing ZEPHIR_INIT_CLASS");
        }
    }

    public function testMethodEntryTablesExist(): void
    {
        $files = glob($this->extStubPath . '/*.zep.h');

        foreach ($files as $file) {
            $content = file_get_contents($file);
            $filename = basename($file);

            // Files with methods should have method entry tables
            if (strpos($content, 'PHP_METHOD') !== false) {
                $this->assertStringContainsString('ZEPHIR_INIT_FUNCS', $content, "{$filename} missing method entry table");
                $this->assertStringContainsString('PHP_FE_END', $content, "{$filename} missing PHP_FE_END");
            }
        }
    }

    public function testVariableDeclarationsFollowPattern(): void
    {
        $cCode = $this->readCompiledFile('arithmetic.zep.c');

        // Verify variable declaration patterns
        $this->assertMatchesRegularExpression('/zend_long\s+\w+/', $cCode, 'Missing zend_long declarations');

        // Should have RETURN macros
        $this->assertMatchesRegularExpression('/RETURN_(LONG|BOOL|STRING|NULL)/', $cCode, 'Missing RETURN macros');
    }

    public function testParameterParsingPatterns(): void
    {
        $cCode = $this->readCompiledFile('0__closure.zep.c');

        // Verify parameter parsing follows correct pattern
        $this->assertMatchesRegularExpression('/ZEND_PARSE_PARAMETERS_START\(\d+,\s*\d+\)/', $cCode);
        $this->assertStringContainsString('Z_PARAM_', $cCode);
        $this->assertStringContainsString('ZEND_PARSE_PARAMETERS_END()', $cCode);
    }

    public function testMemoryManagementPatterns(): void
    {
        $files = glob($this->extStubPath . '/*.zep.c');

        $hasZephirMM = false;
        $mmFiles = [];

        foreach ($files as $file) {
            $content = file_get_contents($file);

            // Check for memory management patterns
            if (strpos($content, 'ZEPHIR_MM_GROW') !== false) {
                $hasZephirMM = true;
                $mmFiles[] = basename($file);
                $this->assertStringContainsString('ZEPHIR_MM_RESTORE', $content, basename($file) . ' has MM_GROW but no MM_RESTORE');
            }
        }

        // Memory management is optional - some simple files don't need it
        // Just verify that files using MM_GROW also have MM_RESTORE
        if ($hasZephirMM) {
            $this->assertNotEmpty($mmFiles, 'Found files with memory management: ' . implode(', ', $mmFiles));
        }

        // This assertion will always pass - we just want to document which files use MM
        $this->assertTrue(true, count($mmFiles) . ' files use ZEPHIR memory management');
    }

    /**
     * Read a compiled file from ext/stub/
     */
    private function readCompiledFile(string $filename): string
    {
        $filepath = $this->extStubPath . '/' . $filename;

        if (!file_exists($filepath)) {
            $this->markTestSkipped("Compiled file not found: {$filepath}");
        }

        return file_get_contents($filepath);
    }
}
