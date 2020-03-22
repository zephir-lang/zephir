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
use Zephir\ClassDefinition;
use Zephir\Variable;

class VariableTest extends TestCase
{
    /** @test */
    public function shouldSetProperties()
    {
        $variable = new Variable('object', 'UnitTest');

        // Check variable default state
        $this->assertFalse($variable->getInitBranch());
        $this->assertSame([], $variable->getInitBranches());

        $this->assertSame('variable', $variable->getType());
        $this->assertFalse($variable->isLocalOnly());
        $this->assertFalse($variable->isReadOnly());
        $this->assertFalse($variable->isTemporal());
        $this->assertTrue($variable->isReusable());
        $this->assertFalse($variable->isIdle());
        $this->assertTrue($variable->isUsed());
        $this->assertFalse($variable->isDoublePointer());
        $this->assertTrue($variable->isMemoryTracked());
        $this->assertSame('UnitTest', $variable->getRealName());
        $this->assertSame('UnitTest', $variable->getName());
        $this->assertNull($variable->getLastUsedNode());
        $this->assertNull($variable->getBranch());
        $this->assertSame(['file' => 'unknown', 'line' => 0, 'char' => 0], $variable->getOriginal());
        $this->assertSame([], $variable->getClassTypes());
        $this->assertNull($variable->getAssociatedClass());
        $this->assertSame(['unknown' => true], $variable->getDynamicTypes());

        // Set properties and check
        $variable->setType('callable');
        $this->assertSame('callable', $variable->getType());

        $variable->setLocalOnly(true);
        $this->assertTrue($variable->isLocalOnly());

        $variable->setReadOnly(true);
        $this->assertTrue($variable->isReadOnly());

        $variable->setTemporal(true);
        $this->assertTrue($variable->isTemporal());

        $variable->setIdle(true);
        $this->assertTrue($variable->isIdle());

        $variable->setReusable(false);
        $this->assertFalse($variable->isReusable());

        $variable->setUsed(false, []);
        $this->assertFalse($variable->isUsed());

        $variable->setUsed(true, ['lastUsedNode']);
        $this->assertSame(['lastUsedNode'], $variable->getLastUsedNode());

        $variable->setIsDoublePointer(true);
        $this->assertTrue($variable->isDoublePointer());

        $variable->setMemoryTracked(false);
        $this->assertFalse($variable->isMemoryTracked());

        $variable->setLowName('unit_test');
        $this->assertSame('UnitTest', $variable->getRealName());
        $this->assertSame('unit_test', $variable->getName());

        $variable->setOriginal(['file' => 'unknown', 'line' => 1, 'char' => 1]);
        $this->assertSame(['file' => 'unknown', 'line' => 1, 'char' => 1], $variable->getOriginal());

        $variable->setClassTypes(['class1', 'class2']);
        $variable->setClassTypes('class3');
        $variable->setClassTypes('class1');
        $this->assertSame(['class1', 'class2', 'class3'], $variable->getClassTypes());

        $classDefinition = new ClassDefinition('Zephir\Test', 'VariableTest');
        $variable->setAssociatedClass($classDefinition);
        $this->assertSame($classDefinition, $variable->getAssociatedClass());
    }
}
