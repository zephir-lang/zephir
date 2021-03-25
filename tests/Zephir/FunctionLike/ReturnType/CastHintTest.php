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

namespace Zephir\Test\FunctionLike\ReturnType;

use PHPUnit\Framework\TestCase;
use Zephir\FunctionLike\ReturnType\CastHint;
use Zephir\FunctionLike\ReturnType\TypeInterface;
use Zephir\Types;

final class CastHintTest extends TestCase
{
    public function testShouldReturnType(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertEquals(TypeInterface::TYPE_PARAMETER, $type->getType());

        $type = new CastHint(Types::T_OBJECT, 'Class2', TypeInterface::TYPE_ANNOTATION);
        $this->assertEquals(TypeInterface::TYPE_ANNOTATION, $type->getType());

        $type = new CastHint(Types::T_OBJECT, 'Class3', 'return-type-custom');
        $this->assertEquals('return-type-custom', $type->getType());
    }

    public function testShouldReturnDataType(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertEquals(Types::T_OBJECT, $type->getDataType());

        $type = new CastHint(Types::T_UNDEFINED, 'Class2');
        $this->assertEquals(Types::T_UNDEFINED, $type->getDataType());
    }

    public function testShouldReturnValue(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertEquals('Class1', $type->getValue());

        $type = new CastHint(Types::T_OBJECT, '\A\Namespaced\Class\Foo');
        $this->assertEquals('\A\Namespaced\Class\Foo', $type->getValue());
    }

    public function testShouldAlwaysBeNotMandatory(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertFalse($type->isMandatory());

        $type = new CastHint(Types::T_OBJECT, 'Class2', TypeInterface::TYPE_ANNOTATION);
        $this->assertFalse($type->isMandatory());
    }

    public function testShouldDetectCollectionNature(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertFalse($type->isCollection());

        $type = new CastHint(Types::T_OBJECT, 'Class1', TypeInterface::TYPE_ANNOTATION, true);
        $this->assertTrue($type->isCollection());

        $type = new CastHint(Types::T_OBJECT, 'Class1', TypeInterface::TYPE_PARAMETER, true);
        $this->assertTrue($type->isCollection());

        $type = new CastHint(Types::T_OBJECT, 'Class1', TypeInterface::TYPE_PARAMETER, false);
        $this->assertFalse($type->isCollection());
    }

    public function testShouldAlwaysBeRealType(): void
    {
        $type = new CastHint(Types::T_OBJECT, 'Class1');
        $this->assertFalse($type->isRealType());
    }
}
