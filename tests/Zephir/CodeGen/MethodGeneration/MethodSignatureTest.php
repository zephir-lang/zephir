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

namespace Zephir\Test\CodeGen\MethodGeneration;

use Zephir\Test\CodeGen\CodeGenTestCase;
use Zephir\Variable\Variable;

/**
 * Tests method signature generation.
 *
 * Verifies PHP_METHOD macro, parameter parsing, and variable declarations.
 */
final class MethodSignatureTest extends CodeGenTestCase
{
    public function testAssignLongGeneratesZvalLong(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        $this->backend->assignLong($var, 42, $this->context);

        $this->assertOutputContains('ZVAL_LONG');
        $this->assertOutputContains('x');
        $this->assertOutputContains('42');
    }

    public function testAssignBoolTrueGeneratesZvalBool(): void
    {
        $var = new Variable('variable', 'flag', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'flag', $this->context);

        $this->backend->assignBool($var, 'true', $this->context);

        $this->assertOutputContains('ZVAL_BOOL');
        $this->assertOutputContains('flag');
        $this->assertOutputContains('true');
    }

    public function testAssignBoolFalseGeneratesZvalBool(): void
    {
        $var = new Variable('variable', 'flag', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'flag', $this->context);

        $this->backend->assignBool($var, 'false', $this->context);

        $this->assertOutputContains('ZVAL_BOOL');
        $this->assertOutputContains('flag');
        $this->assertOutputContains('false');
    }

    public function testAssignDoubleGeneratesZvalDouble(): void
    {
        $var = new Variable('variable', 'pi', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'pi', $this->context);

        $this->backend->assignDouble($var, 3.14159, $this->context);

        $this->assertOutputContains('ZVAL_DOUBLE');
        $this->assertOutputContains('pi');
        $this->assertOutputContains('3.14159');
    }

    public function testAssignStringGeneratesZvalString(): void
    {
        $var = new Variable('variable', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'str', $this->context);

        $this->backend->assignString($var, 'hello', $this->context);

        $this->assertOutputContains('ZVAL_STRING');
        $this->assertOutputContains('str');
        $this->assertOutputContains('hello');
    }

    public function testAssignStringWithEscaping(): void
    {
        $var = new Variable('variable', 'str', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'str', $this->context);

        $this->backend->assignString($var, 'hello\\nworld', $this->context);

        $output = $this->getOutput();
        $this->assertOutputContains('ZVAL_STRING');
        // Should preserve escaped newline
        $this->assertStringContainsString('\\n', $output);
    }

    public function testAssignNullGeneratesZvalNull(): void
    {
        $var = new Variable('variable', 'empty', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'empty', $this->context);

        $this->backend->assignNull($var, $this->context);

        $this->assertOutputContains('ZVAL_NULL');
        $this->assertOutputContains('empty');
    }

    public function testInitVarGeneratesZephirInitVar(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        $this->backend->initVar($var, $this->context);

        $this->assertOutputContains('ZEPHIR_INIT_VAR');
        $this->assertOutputContains('x');
    }

    public function testInitVarSecondTimeGeneratesZephirInitNvar(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        $this->backend->initVar($var, $this->context, true, true);

        $this->assertOutputContains('ZEPHIR_INIT_NVAR');
        $this->assertOutputContains('x');
    }

    public function testInitArrayGeneratesArrayInit(): void
    {
        $var = new Variable('variable', 'arr', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'arr', $this->context);

        $this->backend->initArray($var, $this->context);

        $this->assertOutputContains('array_init');
        $this->assertOutputContains('arr');
    }

    public function testAssignmentSequence(): void
    {
        $var = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'x', $this->context);

        // Typical pattern: init then assign
        $this->backend->initVar($var, $this->context);
        $this->backend->assignLong($var, 42, $this->context);

        $output = $this->getOutput();

        // Should have both operations
        $this->assertOutputContains('ZEPHIR_INIT_VAR');
        $this->assertOutputContains('ZVAL_LONG');

        // Init should come before assign
        $this->assertOutputContainsInOrder([
            'ZEPHIR_INIT_VAR',
            'ZVAL_LONG',
        ]);
    }

    public function testVariableCodeGeneration(): void
    {
        $var = new Variable('variable', 'test', $this->context->branchManager->getCurrentBranch());
        $this->context->symbolTable->addVariable('variable', 'test', $this->context);

        $code = $this->backend->getVariableCode($var);

        // Should reference the zval by address
        $this->assertStringContainsString('test', $code);
    }

    public function testReturnLongMacro(): void
    {
        $this->codePrinter->output('RETURN_LONG(42);');

        $this->assertOutputContains('RETURN_LONG');
        $this->assertOutputContains('42');
    }

    public function testReturnStringMacro(): void
    {
        $this->codePrinter->output('RETURN_STRING("hello");');

        $this->assertOutputContains('RETURN_STRING');
        $this->assertOutputContains('hello');
    }

    public function testReturnBoolMacro(): void
    {
        $this->codePrinter->output('RETURN_BOOL(1);');

        $this->assertOutputContains('RETURN_BOOL');
    }

    public function testReturnNullMacro(): void
    {
        $this->codePrinter->output('RETURN_NULL();');

        $this->assertOutputContains('RETURN_NULL');
    }

    public function testComplexMethodSignatureStructure(): void
    {
        // Simulate a method signature with parameters
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, testMethod)');
        $this->codePrinter->output('{');
        $this->codePrinter->increaseLevel();

        // Variable declarations
        $this->codePrinter->output('zval *x, x_sub;');

        // Parameter parsing
        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_START(1, 1)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('Z_PARAM_ZVAL(x)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_PARSE_PARAMETERS_END();');

        // Method body
        $this->codePrinter->output('RETURN_LONG(42);');

        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('}');

        // Verify structure
        $this->assertOutputContainsInOrder([
            'PHP_METHOD',
            'Stub_TestClass',
            'testMethod',
            'zval *x',
            'ZEND_PARSE_PARAMETERS_START',
            'Z_PARAM_ZVAL',
            'ZEND_PARSE_PARAMETERS_END',
            'RETURN_LONG',
        ]);
    }
}
