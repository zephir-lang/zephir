<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\TypeHinting;

use PHPUnit\Framework\SkippedTestError;
use PHPUnit\Framework\TestCase;
use TestConcreteClass;

class AbstractTest extends TestCase
{
    /**
     * {@inheritdoc}
     */
    public static function setUpBeforeClass()
    {
        if (\PHP_VERSION_ID < 70000) {
            throw new SkippedTestError('Scalar type hint are avalibale in PHP 7 only');
        }
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1667
     */
    public function shouldCreateCompatibleChildClassUsingTypeHintedParams()
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
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1667
     */
    public function shouldCreateCompatibleChildClassWithMultipleReturnTypesHint()
    {
        $childClass = new TestConcreteClass();
        $this->assertEquals('TestConcreteClass::returnOneOfScalar', $childClass->returnOneOfScalar());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1667
     */
    public function shouldCreateCompatibleChildClassWithScalarType()
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
