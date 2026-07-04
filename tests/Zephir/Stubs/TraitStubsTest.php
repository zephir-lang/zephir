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

namespace Zephir\Test\Stubs;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Stubs\Generator;

/**
 * IDE stubs for traits: the trait itself renders with the `trait` keyword,
 * a using class renders `use X;` lines, and members merged in from traits
 * are not re-listed on the using class (zephir#504).
 */
final class TraitStubsTest extends TestCase
{
    private \ReflectionClass $generatorClass;
    private Generator $testClass;

    protected function setUp(): void
    {
        $this->generatorClass = new \ReflectionClass(Generator::class);
        $this->testClass      = new Generator([]);
    }

    private function buildClass(Definition $definition): string
    {
        $method = $this->generatorClass->getMethod('buildClass');
        $method->setAccessible(true);

        return $method->invokeArgs($this->testClass, [$definition, '    ', '']);
    }

    public function testRendersTraitKeyword(): void
    {
        $definition = new Definition('Stub\Traits', 'Greeter');
        $definition->setType(Definition::TYPE_TRAIT);
        $definition->setAliasManager(new AliasManager());
        $definition->addMethod(new Method($definition, ['public'], 'greet', null));

        $source = $this->buildClass($definition);

        $this->assertStringContainsString('trait Greeter', $source);
        $this->assertStringNotContainsString('final', $source);
        $this->assertStringNotContainsString('abstract trait', $source);
        $this->assertStringContainsString('function greet', $source);
    }

    public function testUsingClassRendersUseLinesAndSkipsMergedMembers(): void
    {
        $definition = new Definition('Stub\Traits', 'CounterUser');
        $definition->setAliasManager(new AliasManager());
        $definition->setUsedTraits(['Stub\Traits\Counter']);

        $definition->addMethod(new Method($definition, ['public'], 'own', null));

        // Simulates what the TraitMerger does before stub generation
        $definition->addMethod(new Method($definition, ['public'], 'increment', null));
        $definition->addMergedMemberName('methods', 'increment');

        $source = $this->buildClass($definition);

        $this->assertStringContainsString('use \Stub\Traits\Counter;', $source);
        $this->assertStringContainsString('function own', $source);
        $this->assertStringNotContainsString('function increment', $source);
    }
}
