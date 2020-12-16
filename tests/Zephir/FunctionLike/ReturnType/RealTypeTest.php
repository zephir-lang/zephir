<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\FunctionLike\ReturnType;

use PHPUnit\Framework\TestCase;
use Zephir\FunctionLike\ReturnType\RealType;
use Zephir\FunctionLike\ReturnType\TypeInterface;
use Zephir\Types;

class RealTypeTest extends TestCase
{
    public function testShouldReturnType()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertEquals(TypeInterface::TYPE_PARAMETER, $type->getType());

        $type = new RealType(Types::T_STRING, false, TypeInterface::TYPE_ANNOTATION);
        $this->assertEquals(TypeInterface::TYPE_ANNOTATION, $type->getType());

        $type = new RealType(Types::T_STRING, false, 'return-type-custom');
        $this->assertEquals('return-type-custom', $type->getType());
    }

    public function testShouldReturnMandatoryStatus()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertFalse($type->isMandatory());

        $type = new RealType(Types::T_STRING, false);
        $this->assertFalse($type->isMandatory());

        $type = new RealType(Types::T_STRING, true);
        $this->assertTrue($type->isMandatory());
    }

    public function testShouldAlwaysHaveNullableValue()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertNull($type->getValue());
    }

    public function testShouldAlwaysBeNotACollection()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertFalse($type->isCollection());

        $type = new RealType(Types::T_ARRAY);
        $this->assertFalse($type->isCollection());
    }

    public function testShouldReturnDataType()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertEquals(Types::T_STRING, $type->getDataType());

        $type = new RealType(Types::T_INT, false, TypeInterface::TYPE_ANNOTATION);
        $this->assertEquals(Types::T_INT, $type->getDataType());

        $type = new RealType(Types::T_VOID, true, 'return-type-custom');
        $this->assertEquals(Types::T_VOID, $type->getDataType());
    }

    public function testShouldAlwaysBeRealType()
    {
        $type = new RealType(Types::T_STRING);
        $this->assertTrue($type->isRealType());
    }
}
