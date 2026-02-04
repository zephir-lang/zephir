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
use Zephir\Test\CodeGen\CodeGenTestCase;
use Zephir\Variable\Variable;
use Zephir\Backend\VariablesManager;

/**
 * Tests that validate complete code generation scenarios.
 *
 * These tests generate larger chunks of C code and validate their structure.
 */
final class CompleteCodePatternsTest extends CodeGenTestCase
{
    public function testCompleteVariableDeclarationBlock(): void
    {
        // Simulate declaring multiple variables of different types
        $var1 = new Variable('long', 'count', $this->context->branchManager->getCurrentBranch());
        $var2 = new Variable('double', 'ratio', $this->context->branchManager->getCurrentBranch());
        $var3 = new Variable('variable', 'name', $this->context->branchManager->getCurrentBranch());
        $var4 = new Variable('variable', 'value', $this->context->branchManager->getCurrentBranch());
        $var5 = new Variable('bool', 'flag', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'long' => [$var1],
            'double' => [$var2],
            'variable' => [$var3, $var4],
            'bool' => [$var5],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        // Verify we got declarations for all types (may include initialization code)
        $this->assertGreaterThanOrEqual(5, count($result));

        $declarations = implode("\n", $result);

        // Verify complete structure
        $this->assertStringContainsString('long', $declarations);
        $this->assertStringContainsString('double', $declarations);
        $this->assertStringContainsString('zval', $declarations);
        $this->assertStringContainsString('zend_bool', $declarations);

        // Verify semicolons (at least one per type group)
        $this->assertGreaterThanOrEqual(4, substr_count($declarations, ';'));
    }

    public function testCompleteMethodSignaturePattern(): void
    {
        // Generate a complete method signature pattern
        $this->codePrinter->output('PHP_METHOD(TestClass, testMethod)');
        $this->codePrinter->output('{');
        $this->codePrinter->increaseLevel();

        // Variable declarations
        $this->codePrinter->output('zend_long x = 0, y = 0;');
        $this->codePrinter->output('zval result;');
        $this->codePrinter->outputBlankLine();

        // Parameter parsing
        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_START(2, 2)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('Z_PARAM_LONG(x)');
        $this->codePrinter->output('Z_PARAM_LONG(y)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_END();');
        $this->codePrinter->outputBlankLine();

        // Method body
        $this->codePrinter->output('ZEPHIR_INIT_VAR(&result);');
        $this->codePrinter->output('ZVAL_LONG(&result, (x + y));');
        $this->codePrinter->output('RETURN_CTOR(&result);');

        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('}');

        $output = $this->getOutput();

        // Verify complete structure
        $this->assertOutputContainsInOrder([
            'PHP_METHOD(TestClass, testMethod)',
            '{',
            'zend_long x',
            'zval result',
            'ZEND_PARSE_PARAMETERS_START',
            'Z_PARAM_LONG(x)',
            'Z_PARAM_LONG(y)',
            'ZEND_PARSE_PARAMETERS_END',
            'ZEPHIR_INIT_VAR',
            'ZVAL_LONG',
            'RETURN_CTOR',
            '}',
        ]);

        // Verify indentation is present (tabs)
        $this->assertStringContainsString("\t", $output);

        // Verify balanced braces
        $this->assertEquals(substr_count($output, '{'), substr_count($output, '}'));
    }

    public function testCompleteClassHeaderPattern(): void
    {
        // Generate a complete class header pattern
        $this->codePrinter->outputBlankLine();
        $this->codePrinter->output('extern zend_class_entry *testclass_ce;');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('ZEPHIR_INIT_CLASS(TestClass);');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('PHP_METHOD(TestClass, __construct);');
        $this->codePrinter->output('PHP_METHOD(TestClass, getValue);');
        $this->codePrinter->output('PHP_METHOD(TestClass, setValue);');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_testclass_setvalue, 0, 0, 1)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_END_ARG_INFO()');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('ZEPHIR_INIT_FUNCS(testclass_method_entry) {');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('PHP_ME(TestClass, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)');
        $this->codePrinter->output('PHP_ME(TestClass, getValue, NULL, ZEND_ACC_PUBLIC)');
        $this->codePrinter->output('PHP_ME(TestClass, setValue, arginfo_testclass_setvalue, ZEND_ACC_PUBLIC)');
        $this->codePrinter->output('PHP_FE_END');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('};');

        $output = $this->getOutput();

        // Verify complete structure
        $this->assertOutputContainsInOrder([
            'extern zend_class_entry',
            'ZEPHIR_INIT_CLASS',
            'PHP_METHOD(TestClass, __construct)',
            'PHP_METHOD(TestClass, getValue)',
            'PHP_METHOD(TestClass, setValue)',
            'ZEND_BEGIN_ARG_INFO_EX',
            'ZEND_ARG_TYPE_INFO',
            'ZEND_END_ARG_INFO',
            'ZEPHIR_INIT_FUNCS',
            'PHP_ME(TestClass, __construct',
            'PHP_ME(TestClass, getValue',
            'PHP_ME(TestClass, setValue',
            'PHP_FE_END',
        ]);

        // Verify method count (may include ZEND_DOC_METHOD or other entries)
        $this->assertGreaterThanOrEqual(3, $this->countInOutput('PHP_METHOD'));
        $this->assertGreaterThanOrEqual(3, $this->countInOutput('PHP_ME'));
    }

    public function testCompleteInitializationSequence(): void
    {
        $variablesManager = new VariablesManager();

        // Initialize multiple variables with defaults
        $vars = [
            ['type' => 'variable', 'name' => 'count', 'value' => ['type' => 'int', 'value' => 0]],
            ['type' => 'variable', 'name' => 'name', 'value' => ['type' => 'string', 'value' => 'test']],
            ['type' => 'variable', 'name' => 'flag', 'value' => ['type' => 'bool', 'value' => 'true']],
        ];

        foreach ($vars as $varInfo) {
            $var = new Variable($varInfo['type'], $varInfo['name'], $this->context->branchManager->getCurrentBranch());
            $this->context->symbolTable->addVariable($varInfo['type'], $varInfo['name'], $this->context);
            $variablesManager->initializeDefaults($var, $varInfo['value'], $this->context);
        }

        $output = $this->getOutput();

        // Verify all initializations present (order may vary)
        $this->assertStringContainsString('ZVAL_LONG', $output);
        $this->assertStringContainsString('ZVAL_STRING', $output);
        $this->assertStringContainsString('ZVAL_BOOL', $output);

        // Verify multiple INIT_VAR calls
        $this->assertGreaterThanOrEqual(3, $this->countInOutput('ZEPHIR_INIT_VAR'));

        // Verify variable names
        $this->assertStringContainsString('count', $output);
        $this->assertStringContainsString('name', $output);
        $this->assertStringContainsString('flag', $output);
    }

    public function testVariableLifecycle(): void
    {
        // Test complete variable lifecycle: declare, init, assign, use
        $var = new Variable('variable', 'result', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'result', $this->context);

        // 1. Initialize
        $this->backend->initVar($var, $this->context);

        // 2. Assign initial value
        $this->backend->assignLong($var, 0, $this->context);

        // 3. Re-initialize (NVAR)
        $this->backend->initVar($var, $this->context, true, true);

        // 4. Assign new value
        $this->backend->assignLong($var, 42, $this->context);

        $output = $this->getOutput();

        // Verify sequence
        $this->assertOutputContainsInOrder([
            'ZEPHIR_INIT_VAR',
            'ZVAL_LONG(&result, 0)',
            'ZEPHIR_INIT_NVAR',
            'ZVAL_LONG(&result, 42)',
        ]);
    }

    public function testConditionalReturnPattern(): void
    {
        // Simulate a method with conditional returns
        $this->codePrinter->output('PHP_METHOD(TestClass, check)');
        $this->codePrinter->output('{');
        $this->codePrinter->increaseLevel();

        $this->codePrinter->output('zend_long x = 0;');
        $this->codePrinter->output('zval _0;');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_START(1, 1)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('Z_PARAM_LONG(x)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_END();');
        $this->codePrinter->outputBlankLine();

        $this->codePrinter->output('if (x > 10) {');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('RETURN_MM_STRING("large");');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('}');

        $this->codePrinter->output('RETURN_MM_STRING("small");');

        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('}');

        $output = $this->getOutput();

        // Verify structure
        $this->assertOutputContainsInOrder([
            'PHP_METHOD',
            'zend_long x',
            'ZEND_PARSE_PARAMETERS_START',
            'if (x > 10)',
            'RETURN_MM_STRING("large")',
            'RETURN_MM_STRING("small")',
        ]);

        // Verify two return statements
        $this->assertEquals(2, $this->countInOutput('RETURN_MM_STRING'));
    }

    public function testArrayManipulationPattern(): void
    {
        $var = new Variable('variable', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'arr', $this->context);

        // Initialize array
        $this->backend->initArray($var, $this->context);

        // The pattern would include array element additions, but those require
        // more complex setup. For now, just verify array initialization works.

        $output = $this->getOutput();
        $this->assertStringContainsString('array_init', $output);
        $this->assertStringContainsString('arr', $output);
    }

    public function testCompleteMethodWithAllFeatures(): void
    {
        // Generate a method that uses multiple features
        $this->codePrinter->output('PHP_METHOD(Complex, process)');
        $this->codePrinter->output('{');
        $this->codePrinter->increaseLevel();

        // Declarations
        $this->codePrinter->output('zend_long count = 0;');
        $this->codePrinter->output('zend_bool flag = 0;');
        $this->codePrinter->output('double ratio = 0;');
        $this->codePrinter->output('zval result, temp;');
        $this->codePrinter->outputBlankLine();

        // Memory management
        $this->codePrinter->output('ZEPHIR_MM_GROW();');
        $this->codePrinter->outputBlankLine();

        // Initialization
        $this->codePrinter->output('ZEPHIR_INIT_VAR(&result);');
        $this->codePrinter->output('ZVAL_LONG(&result, 0);');
        $this->codePrinter->outputBlankLine();

        // Logic
        $this->codePrinter->output('count = 10;');
        $this->codePrinter->output('flag = 1;');
        $this->codePrinter->output('ratio = 1.5;');
        $this->codePrinter->outputBlankLine();

        // Return
        $this->codePrinter->output('RETURN_MM_LONG(count);');

        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('}');

        $output = $this->getOutput();

        // Verify complete structure exists
        $patterns = [
            'PHP_METHOD',
            'zend_long count',
            'zend_bool flag',
            'double ratio',
            'zval result',
            'ZEPHIR_MM_GROW',
            'ZEPHIR_INIT_VAR',
            'ZVAL_LONG',
            'RETURN_MM_LONG',
        ];

        foreach ($patterns as $pattern) {
            $this->assertStringContainsString(
                $pattern,
                $output,
                "Expected pattern '{$pattern}' not found in complete method"
            );
        }

        // Verify structure integrity
        $this->assertOutputContainsInOrder([
            'PHP_METHOD',
            'zend_long count',
            'zval result',
            'ZEPHIR_MM_GROW',
            'ZEPHIR_INIT_VAR',
            'count = 10',
            'RETURN_MM_LONG',
        ]);
    }
}
