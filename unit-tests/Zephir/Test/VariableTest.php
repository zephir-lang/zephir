<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Branch;
use Zephir\BranchManager;
use Zephir\ClassDefinition;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Variable;

class VariableTest extends TestCase
{
    /** @var Variable */
    private $zephirVar;

    public function setUp()
    {
        $this->zephirVar = new Variable('object', 'UnitTest');
    }

    /** @test */
    public function shouldSetProperties()
    {
        // Check variable default state
        $this->assertFalse($this->zephirVar->getInitBranch());
        $this->assertSame([], $this->zephirVar->getInitBranches());

        $this->assertSame('variable', $this->zephirVar->getType());
        $this->assertFalse($this->zephirVar->isLocalOnly());
        $this->assertFalse($this->zephirVar->isReadOnly());
        $this->assertFalse($this->zephirVar->isTemporal());
        $this->assertTrue($this->zephirVar->isReusable());
        $this->assertFalse($this->zephirVar->isIdle());
        $this->assertTrue($this->zephirVar->isUsed());
        $this->assertFalse($this->zephirVar->isDoublePointer());
        $this->assertTrue($this->zephirVar->isMemoryTracked());
        $this->assertSame('UnitTest', $this->zephirVar->getRealName());
        $this->assertSame('UnitTest', $this->zephirVar->getName());
        $this->assertNull($this->zephirVar->getLastUsedNode());
        $this->assertNull($this->zephirVar->getBranch());
        $this->assertSame(['file' => 'unknown', 'line' => 0, 'char' => 0], $this->zephirVar->getOriginal());
        $this->assertSame([], $this->zephirVar->getClassTypes());
        $this->assertNull($this->zephirVar->getAssociatedClass());
        $this->assertSame(['unknown' => true], $this->zephirVar->getDynamicTypes());
        $this->assertFalse($this->zephirVar->hasAnyDynamicType('string'));
        $this->assertTrue($this->zephirVar->hasAnyDynamicType(['string', 'unknown']));
        $this->assertFalse($this->zephirVar->hasAnyDynamicType([]));
        $this->assertTrue($this->zephirVar->hasDifferentDynamicType(['string', 'int']));
        $this->assertSame(0, $this->zephirVar->getNumberUses());
        $this->assertSame(0, $this->zephirVar->getNumberMutations());
        $this->assertFalse($this->zephirVar->isInitialized());
        $this->assertFalse($this->zephirVar->isExternal());
        $this->assertFalse($this->zephirVar->mustInitNull());
        $this->assertNull($this->zephirVar->getDefaultInitValue());
        $this->assertSame(0, $this->zephirVar->getSkipVariant());
        $this->assertSame(0, $this->zephirVar->getVariantInits());
        $this->assertTrue($this->zephirVar->isVariable());
        $this->assertFalse($this->zephirVar->isLocalStatic());
        $this->assertFalse($this->zephirVar->isSuperGlobal());

        // Set properties and check
        $this->zephirVar->setType('callable');
        $this->assertSame('callable', $this->zephirVar->getType());

        $this->zephirVar->setLocalOnly(true);
        $this->assertTrue($this->zephirVar->isLocalOnly());

        $this->zephirVar->setReadOnly(true);
        $this->assertTrue($this->zephirVar->isReadOnly());

        $this->zephirVar->setTemporal(true);
        $this->assertTrue($this->zephirVar->isTemporal());

        $this->zephirVar->setIdle(true);
        $this->assertTrue($this->zephirVar->isIdle());

        $this->zephirVar->setReusable(false);
        $this->assertFalse($this->zephirVar->isReusable());

        $this->zephirVar->setUsed(false, []);
        $this->assertFalse($this->zephirVar->isUsed());

        $this->zephirVar->setUsed(true, ['lastUsedNode']);
        $this->assertSame(['lastUsedNode'], $this->zephirVar->getLastUsedNode());

        $this->zephirVar->setIsDoublePointer(true);
        $this->assertTrue($this->zephirVar->isDoublePointer());

        $this->zephirVar->setMemoryTracked(false);
        $this->assertFalse($this->zephirVar->isMemoryTracked());

        $this->zephirVar->setLowName('unit_test');
        $this->assertSame('UnitTest', $this->zephirVar->getRealName());
        $this->assertSame('unit_test', $this->zephirVar->getName());

        $this->zephirVar->setOriginal(['file' => 'unknown', 'line' => 1, 'char' => 1]);
        $this->assertSame(['file' => 'unknown', 'line' => 1, 'char' => 1], $this->zephirVar->getOriginal());

        $this->zephirVar->setClassTypes(['class1', 'class2']);
        $this->zephirVar->setClassTypes('class3');
        $this->zephirVar->setClassTypes('class1');
        $this->assertSame(['class1', 'class2', 'class3'], $this->zephirVar->getClassTypes());

        $classDefinition = new ClassDefinition('Zephir\Test', 'VariableTest');
        $this->zephirVar->setAssociatedClass($classDefinition);
        $this->assertSame($classDefinition, $this->zephirVar->getAssociatedClass());

        $this->zephirVar->setDynamicTypes(['type' => 'int']);
        $this->zephirVar->setDynamicTypes('uint');
        $this->assertSame(['int' => true, 'uint' => true], $this->zephirVar->getDynamicTypes());

        $this->zephirVar->increaseUses();
        $this->assertSame(1, $this->zephirVar->getNumberUses());

        $this->zephirVar->increaseMutates();
        $this->assertSame(1, $this->zephirVar->getNumberMutations());

        $context = new CompilationContext();
        $context->branchManager = new BranchManager();
        $context->branchManager->addBranch(new Branch());
        $this->zephirVar->setIsInitialized(false, $context);
        $this->assertFalse($this->zephirVar->isInitialized());
        $this->zephirVar->setIsInitialized(true, $context);
        $this->assertTrue($this->zephirVar->isInitialized());

        $this->zephirVar->setIsExternal(true);
        $this->assertTrue($this->zephirVar->isExternal());

        $this->zephirVar->setMustInitNull(true);
        $this->assertTrue($this->zephirVar->mustInitNull());

        $this->zephirVar->setDefaultInitValue('testValue');
        $this->assertSame('testValue', $this->zephirVar->getDefaultInitValue());

        $this->zephirVar->skipInitVariant(42);
        $this->assertSame(42, $this->zephirVar->getSkipVariant());

        $this->zephirVar->increaseVariantIfNull();
        $this->zephirVar->increaseVariantIfNull();
        $this->assertSame(3, $this->zephirVar->getVariantInits());

        $this->zephirVar->setType('not-a-type');
        $this->assertFalse($this->zephirVar->isVariable());

        $this->zephirVar->setLocalOnly(true);
        $this->zephirVar->setIsExternal(true);
        $this->assertTrue($this->zephirVar->isLocalStatic());

        $superglobalVar = new Variable('', '_SERVER');
        $superglobalVar->setIsExternal(true);
        $this->assertTrue($superglobalVar->isSuperGlobal());
    }

    /** @test */
    public function shouldEnableDefaultAutoInitValue()
    {
        $numericTypes = [
            'char', 'boolean', 'bool', 'int', 'uint', 'long', 'ulong',
            'double', 'zephir_ce_guard',
        ];

        $nullableTypes = [
            'variable', 'string', 'array',
        ];

        $delegate = function (Variable $zephirVar, string $type) {
            $zephirVar->setDefaultInitValue('default');
            $zephirVar->setType($type);
            $zephirVar->enableDefaultAutoInitValue();
        };

        foreach ($numericTypes as $type) {
            $delegate($this->zephirVar, $type);

            $this->assertSame(0, $this->zephirVar->getDefaultInitValue());
        }

        foreach ($nullableTypes as $type) {
            $delegate($this->zephirVar, $type);

            $this->assertNull($this->zephirVar->getDefaultInitValue());
        }

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('Cannot create an automatic safe default value for variable type: not-a-type');

        $this->zephirVar->setType('not-a-type');
        $this->zephirVar->enableDefaultAutoInitValue();
    }
}
