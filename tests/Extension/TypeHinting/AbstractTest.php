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

namespace Extension\TypeHinting;

use PHPUnit\Framework\TestCase;
use TestConcreteClass;

final class AbstractTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1667
     */
    public function testShouldCreateCompatibleChildClassUsingTypeHintedParams(): void
    {
        $childClass = new TestConcreteClass();

        $this->assertEquals(
            [[1667], '/', true, null],
            $childClass->testFunc([1667], '/', true, null)
        );

        $this->assertEquals(
            [['issue'], '\\', true, 1667],
            $childClass->testFunc(['issue'], '\\', true, 1667)
        );
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1667
     */
    public function testShouldCreateCompatibleChildClassWithMultipleReturnTypesHint(): void
    {
        $childClass = new TestConcreteClass();
        $this->assertEquals('TestConcreteClass::returnOneOfScalar', $childClass->returnOneOfScalar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1667
     */
    public function testShouldCreateCompatibleChildClassWithScalarType(): void
    {
        $childClass = new TestConcreteClass();

        $this->assertEquals(1, $childClass->returnInt());
        $this->assertEquals(1, $childClass->returnUint());
        $this->assertEquals(1, $childClass->returnLong());
        $this->assertEquals(1.0, $childClass->returnFloat());
        $this->assertEquals(1.0, $childClass->returnDouble());
        $this->assertEquals('TestConcreteClass::returnString', $childClass->returnString());
        $this->assertTrue($childClass->returnBoolean());
        $this->assertEquals(1, $childClass->returnChar());
    }
}
