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

namespace Zephir\Test\CodeGen\ClassStructure;

use Zephir\Test\CodeGen\CodeGenTestCase;

/**
 * Tests class header file generation.
 *
 * Verifies ZEPHIR_INIT_CLASS, extern declarations, and method signatures.
 */
final class ClassHeaderTest extends CodeGenTestCase
{
    public function testExternClassEntryDeclaration(): void
    {
        $this->codePrinter->output('extern zend_class_entry *stub_testclass_ce;');

        $this->assertOutputContains('extern');
        $this->assertOutputContains('zend_class_entry');
        $this->assertOutputContains('*stub_testclass_ce');
    }

    public function testZephirInitClassDeclaration(): void
    {
        $this->codePrinter->output('ZEPHIR_INIT_CLASS(stub_testclass);');

        $this->assertOutputContains('ZEPHIR_INIT_CLASS');
        $this->assertOutputContains('stub_testclass');
    }

    public function testPhpMethodForwardDeclaration(): void
    {
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, testMethod);');

        $this->assertOutputContains('PHP_METHOD');
        $this->assertOutputContains('Stub_TestClass');
        $this->assertOutputContains('testMethod');
        $this->assertOutputContains(';');
    }

    public function testMultipleMethodForwardDeclarations(): void
    {
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, method1);');
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, method2);');
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, method3);');

        $this->assertOutputContainsInOrder([
            'PHP_METHOD(Stub_TestClass, method1)',
            'PHP_METHOD(Stub_TestClass, method2)',
            'PHP_METHOD(Stub_TestClass, method3)',
        ]);

        $this->assertEquals(3, $this->countInOutput('PHP_METHOD'));
    }

    public function testArgInfoDeclaration(): void
    {
        $this->codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_testclass_testmethod, 0, 0, 1)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('ZEND_ARG_INFO(0, x)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_END_ARG_INFO()');

        $this->assertOutputContainsInOrder([
            'ZEND_BEGIN_ARG_INFO_EX',
            'arginfo_stub_testclass_testmethod',
            'ZEND_ARG_INFO',
            'ZEND_END_ARG_INFO',
        ]);
    }

    public function testArgInfoWithMultipleParameters(): void
    {
        $this->codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_test, 0, 0, 2)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('ZEND_ARG_INFO(0, x)');
        $this->codePrinter->output('ZEND_ARG_INFO(0, y)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_END_ARG_INFO()');

        $this->assertEquals(2, $this->countInOutput('ZEND_ARG_INFO(0,'));
    }

    public function testArgInfoWithTypeHint(): void
    {
        $this->codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_test, 0, 0, 1)');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('ZEND_END_ARG_INFO()');

        $this->assertOutputContains('ZEND_ARG_TYPE_INFO');
        $this->assertOutputContains('IS_LONG');
    }

    public function testMethodEntryTable(): void
    {
        $this->codePrinter->output('ZEPHIR_INIT_FUNCS(stub_testclass_method_entry) {');
        $this->codePrinter->increaseLevel();
        $this->codePrinter->output('PHP_ME(Stub_TestClass, testMethod, arginfo_stub_testclass_testmethod, ZEND_ACC_PUBLIC)');
        $this->codePrinter->output('PHP_FE_END');
        $this->codePrinter->decreaseLevel();
        $this->codePrinter->output('};');

        $this->assertOutputContainsInOrder([
            'ZEPHIR_INIT_FUNCS',
            'stub_testclass_method_entry',
            'PHP_ME',
            'PHP_FE_END',
        ]);
    }

    public function testMethodEntryWithModifiers(): void
    {
        $this->codePrinter->output('PHP_ME(Stub_TestClass, publicMethod, NULL, ZEND_ACC_PUBLIC)');

        $this->assertOutputContains('ZEND_ACC_PUBLIC');
    }

    public function testMethodEntryProtected(): void
    {
        $this->codePrinter->output('PHP_ME(Stub_TestClass, protectedMethod, NULL, ZEND_ACC_PROTECTED)');

        $this->assertOutputContains('ZEND_ACC_PROTECTED');
    }

    public function testMethodEntryStatic(): void
    {
        $this->codePrinter->output('PHP_ME(Stub_TestClass, staticMethod, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)');

        $this->assertOutputContains('ZEND_ACC_STATIC');
        $this->assertOutputContains('ZEND_ACC_PUBLIC');
    }

    public function testMethodEntryFinal(): void
    {
        $this->codePrinter->output('PHP_ME(Stub_TestClass, finalMethod, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)');

        $this->assertOutputContains('ZEND_ACC_FINAL');
    }

    public function testCompleteHeaderStructure(): void
    {
        // Simulate a complete header file structure
        $this->codePrinter->outputBlankLine();
        $this->codePrinter->output('extern zend_class_entry *stub_testclass_ce;');
        $this->codePrinter->outputBlankLine();
        $this->codePrinter->output('ZEPHIR_INIT_CLASS(Stub_TestClass);');
        $this->codePrinter->outputBlankLine();
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, __construct);');
        $this->codePrinter->output('PHP_METHOD(Stub_TestClass, testMethod);');
        $this->codePrinter->outputBlankLine();

        $output = $this->getOutput();

        // Verify structure exists
        $this->assertOutputContains('extern zend_class_entry');
        $this->assertOutputContains('ZEPHIR_INIT_CLASS');
        $this->assertOutputContains('PHP_METHOD(Stub_TestClass, __construct)');
        $this->assertOutputContains('PHP_METHOD(Stub_TestClass, testMethod)');

        // Verify order
        $this->assertOutputContainsInOrder([
            'extern zend_class_entry',
            'ZEPHIR_INIT_CLASS',
            'PHP_METHOD',
        ]);
    }

    public function testZephirApiExportDeclaration(): void
    {
        $this->codePrinter->output('extern ZEPHIR_API zend_class_entry *stub_testclass_ce;');

        $this->assertOutputContains('ZEPHIR_API');
        $this->assertOutputContains('extern');
    }

    public function testClassConstantDeclaration(): void
    {
        $this->codePrinter->output('zephir_declare_class_constant_long(stub_testclass_ce, SL("VERSION"), 1);');

        $this->assertOutputContains('zephir_declare_class_constant_long');
        $this->assertOutputContains('VERSION');
    }

    public function testPropertyDeclaration(): void
    {
        $this->codePrinter->output('zend_declare_property_null(stub_testclass_ce, SL("property"), ZEND_ACC_PROTECTED);');

        $this->assertOutputContains('zend_declare_property_null');
        $this->assertOutputContains('property');
        $this->assertOutputContains('ZEND_ACC_PROTECTED');
    }

    public function testInterfaceImplementation(): void
    {
        $this->codePrinter->output('zend_class_implements(stub_testclass_ce, 1, stub_testinterface_ce);');

        $this->assertOutputContains('zend_class_implements');
        $this->assertOutputContains('stub_testinterface_ce');
    }

    public function testClassExtension(): void
    {
        $this->codePrinter->output('ZEPHIR_REGISTER_CLASS_EX(Stub, TestClass, stub, testclass, stub_baseclass_ce, stub_testclass_method_entry, 0);');

        $this->assertOutputContains('ZEPHIR_REGISTER_CLASS_EX');
        $this->assertOutputContains('stub_baseclass_ce');
    }
}
