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

use Zephir\Test\CodeGen\CodeGenTestCase;
use Zephir\Variable\Variable;

/**
 * Tests Backend::declareVariables() method.
 *
 * Verifies correct C variable declaration generation for different types.
 */
final class DeclareVariablesTest extends CodeGenTestCase
{
    public function testDeclareZvalVariables(): void
    {
        $var1 = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $var2 = new Variable('variable', 'y', $this->context->branchManager->getCurrentBranch());
        $var3 = new Variable('variable', 'z', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'variable' => [$var1, $var2, $var3],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $this->assertIsArray($result);
        $this->assertNotEmpty($result);

        // Should group zval variables on one line
        $declarations = implode("\n", $result);
        $this->assertStringContainsString('zval', $declarations);
        $this->assertStringContainsString('x', $declarations);
        $this->assertStringContainsString('y', $declarations);
        $this->assertStringContainsString('z', $declarations);
    }

    public function testDeclareLongVariables(): void
    {
        $var1 = new Variable('long', 'a', $this->context->branchManager->getCurrentBranch());
        $var2 = new Variable('long', 'b', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'long' => [$var1, $var2],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $this->assertIsArray($result);
        $declarations = implode("\n", $result);

        // Backend uses 'long' in C code, not 'zend_long'
        $this->assertStringContainsString('long', $declarations);
        $this->assertStringContainsString('a', $declarations);
        $this->assertStringContainsString('b', $declarations);
    }

    public function testDeclareIntVariable(): void
    {
        $var = new Variable('int', 'count', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'int' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        // 'int' type is mapped to 'zend_long' in the backend
        $this->assertStringContainsString('zend_long', $declarations);
        $this->assertStringContainsString('count', $declarations);
    }

    public function testDeclareUintVariable(): void
    {
        $var = new Variable('uint', 'flags', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'uint' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        // 'uint' type is mapped to 'zend_ulong' in the backend
        $this->assertStringContainsString('zend_ulong', $declarations);
        $this->assertStringContainsString('flags', $declarations);
    }

    public function testDeclareDoubleVariable(): void
    {
        $var = new Variable('double', 'pi', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'double' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        $this->assertStringContainsString('double', $declarations);
        $this->assertStringContainsString('pi', $declarations);
    }

    public function testDeclareBoolVariable(): void
    {
        $var = new Variable('bool', 'flag', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'bool' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        $this->assertStringContainsString('zend_bool', $declarations);
        $this->assertStringContainsString('flag', $declarations);
    }

    public function testDeclareStringVariable(): void
    {
        $var = new Variable('string', 'str', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'string' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        // 'string' type in Zephir is handled as zval variable
        $this->assertStringContainsString('zval', $declarations);
        $this->assertStringContainsString('str', $declarations);
    }

    public function testDeclarePointerVariable(): void
    {
        $var = new Variable('zend_class_entry', 'ce', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'zend_class_entry' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        // Should include pointer
        $this->assertStringContainsString('zend_class_entry', $declarations);
        $this->assertStringContainsString('*', $declarations);
        $this->assertStringContainsString('ce', $declarations);
    }

    public function testDeclareMixedTypes(): void
    {
        $var1 = new Variable('long', 'a', $this->context->branchManager->getCurrentBranch());
        $var2 = new Variable('double', 'b', $this->context->branchManager->getCurrentBranch());
        $var3 = new Variable('variable', 'x', $this->context->branchManager->getCurrentBranch());
        $var4 = new Variable('variable', 'y', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'long'     => [$var1],
            'double'   => [$var2],
            'variable' => [$var3, $var4],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $this->assertIsArray($result);
        // May include additional initialization lines
        $this->assertGreaterThanOrEqual(3, count($result));

        $declarations = implode("\n", $result);
        $this->assertStringContainsString('long', $declarations);
        $this->assertStringContainsString('double', $declarations);
        $this->assertStringContainsString('zval', $declarations);
    }

    public function testDeclareHashTableVariable(): void
    {
        $var = new Variable('HashTable', 'ht', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'HashTable' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $declarations = implode("\n", $result);
        $this->assertStringContainsString('HashTable', $declarations);
        $this->assertStringContainsString('*', $declarations);
        $this->assertStringContainsString('ht', $declarations);
    }

    public function testDeclareVariablesReturnsArray(): void
    {
        $var = new Variable('int', 'x', $this->context->branchManager->getCurrentBranch());

        $typeToVariables = [
            'int' => [$var],
        ];

        $result = $this->backend->declareVariables(null, $typeToVariables);

        $this->assertIsArray($result);
        $this->assertNotEmpty($result);

        foreach ($result as $declaration) {
            $this->assertIsString($declaration);
            // Each declaration should end with semicolon
            $this->assertStringContainsString(';', $declaration);
        }
    }
}
