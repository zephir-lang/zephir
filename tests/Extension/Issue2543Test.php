<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use ReflectionMethod;
use Stub\Issue2543;

/**
 * A property docblock that includes the variable name (`@var int $name`)
 * together with a `{set}`/`{get}` shortcut must compile, and the generated
 * accessors must use the type only (not the variable name).
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2543
 */
final class Issue2543Test extends TestCase
{
    public function testShortcutsWithTypedDocblockWork(): void
    {
        $object = new Issue2543();

        $object->setVarIntPositive(42);
        $this->assertSame(42, $object->getVarIntPositive());

        $object->setLabel('hello');
        $this->assertSame('hello', $object->getLabel());
    }

    public function testGeneratedAccessorsUseTheTypeNotTheVariableName(): void
    {
        $setter = new ReflectionMethod(Issue2543::class, 'setVarIntPositive');
        $param  = $setter->getParameters()[0];

        $this->assertTrue($param->hasType());
        $this->assertSame('int', (string) $param->getType());

        $getter = new ReflectionMethod(Issue2543::class, 'getVarIntPositive');
        $this->assertSame('int', (string) $getter->getReturnType());
    }
}
