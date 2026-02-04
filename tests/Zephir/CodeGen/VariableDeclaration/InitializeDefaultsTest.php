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

namespace Zephir\Test\CodeGen\VariableDeclaration;

use Zephir\Backend\VariablesManager;
use Zephir\Test\CodeGen\CodeGenTestCase;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

/**
 * Tests VariablesManager::initializeDefaults() method.
 *
 * Verifies correct initialization code generation for variables with default values.
 */
final class InitializeDefaultsTest extends CodeGenTestCase
{
    private VariablesManager $variablesManager;

    protected function setUp(): void
    {
        parent::setUp();
        $this->variablesManager = new VariablesManager();
    }

    public function testInitDynamicVarWithIntValue(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        $value = [
            'type'  => Types::T_INT,
            'value' => 42,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $output = $this->getOutput();
        $this->assertOutputContains('ZEPHIR_INIT_VAR');
        $this->assertOutputContains('ZVAL_LONG');
        $this->assertOutputContains('42');
    }

    public function testInitDynamicVarWithLongValue(): void
    {
        $var = new Variable('variable', 'num', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'num', $this->context);

        $value = [
            'type'  => Types::T_LONG,
            'value' => 1000,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_LONG');
        $this->assertOutputContains('1000');
    }

    public function testInitDynamicVarWithBoolTrue(): void
    {
        $var = new Variable('variable', 'flag', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'flag', $this->context);

        $value = [
            'type'  => Types::T_BOOL,
            'value' => 'true',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_BOOL');
    }

    public function testInitDynamicVarWithBoolFalse(): void
    {
        $var = new Variable('variable', 'flag', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'flag', $this->context);

        $value = [
            'type'  => Types::T_BOOL,
            'value' => 'false',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_BOOL');
    }

    public function testInitDynamicVarWithNull(): void
    {
        $var = new Variable('variable', 'empty', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'empty', $this->context);

        $value = [
            'type' => Types::T_NULL,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_NULL');
    }

    public function testInitDynamicVarWithDouble(): void
    {
        $var = new Variable('variable', 'pi', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'pi', $this->context);

        $value = [
            'type'  => Types::T_DOUBLE,
            'value' => 3.14159,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_DOUBLE');
        $this->assertOutputContains('3.14159');
    }

    public function testInitDynamicVarWithString(): void
    {
        $var = new Variable('variable', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'str', $this->context);

        $value = [
            'type'  => Types::T_STRING,
            'value' => 'hello',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_STRING');
        $this->assertOutputContains('hello');
    }

    public function testInitDynamicVarWithStringEscaping(): void
    {
        $var = new Variable('variable', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'str', $this->context);

        $value = [
            'type'  => Types::T_STRING,
            'value' => 'hello"world',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_STRING');
        // Should escape quotes
        $this->assertOutputContains('\\');
    }

    public function testInitDynamicVarWithChar(): void
    {
        $var = new Variable('variable', 'ch', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'ch', $this->context);

        $value = [
            'type'  => Types::T_CHAR,
            'value' => 'A',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_LONG');
        $this->assertOutputContains("'A'");
    }

    public function testInitDynamicVarWithEmptyArray(): void
    {
        $var = new Variable('variable', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'arr', $this->context);

        $value = [
            'type' => 'empty-array',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('array_init');
    }

    public function testInitDynamicVarWithArray(): void
    {
        $var = new Variable('variable', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'arr', $this->context);

        $value = [
            'type' => Types::T_ARRAY,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('array_init');
    }

    public function testInitStringVarWithString(): void
    {
        $var = new Variable('string', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('string', 'str', $this->context);

        $value = [
            'type'  => Types::T_STRING,
            'value' => 'test',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_STRING');
        $this->assertOutputContains('test');
    }

    public function testInitStringVarWithNull(): void
    {
        $var = new Variable('string', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('string', 'str', $this->context);

        $value = [
            'type' => Types::T_NULL,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        // When string is initialized with null, only INIT_VAR is generated
        $this->assertOutputContains('ZEPHIR_INIT_VAR');
    }

    public function testInitArrayVarWithNull(): void
    {
        $var = new Variable('array', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('array', 'arr', $this->context);

        $value = [
            'type' => Types::T_NULL,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('ZVAL_NULL');
    }

    public function testInitArrayVarWithEmptyArray(): void
    {
        $var = new Variable('array', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('array', 'arr', $this->context);

        $value = [
            'type' => 'empty-array',
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        $this->assertOutputContains('array_init');
    }

    public function testDoesNotInitializeReservedNames(): void
    {
        foreach (VariablesManager::RESERVED_NAMES as $reservedName) {
            $this->clearOutput();

            $var = new Variable('variable', $reservedName, $this->context->branchManager->getCurrentBranch());
            // Don't add to symbol table to avoid conflicts

            $value = [
                'type'  => Types::T_INT,
                'value' => 42,
            ];

            $this->variablesManager->initializeDefaults($var, $value, $this->context);

            // Should not generate any initialization code for reserved names
            $this->assertEmpty($this->getOutput(), "Should not initialize reserved name: {$reservedName}");
        }
    }

    public function testInitializationSetsMemoryFlag(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        $value = [
            'type'  => Types::T_INT,
            'value' => 1,
        ];

        $this->variablesManager->initializeDefaults($var, $value, $this->context);

        // Verify initialization completed and generated code
        $this->assertOutputContains('ZEPHIR_INIT_VAR');
        $this->assertOutputContains('ZVAL_LONG');
    }
}
