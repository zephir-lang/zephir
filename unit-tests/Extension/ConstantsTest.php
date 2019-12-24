<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Constants;
use Test\Oo\ConstantsInterface;

class ConstantsTest extends TestCase
{
    const EXPECTED_DOUBLE_DELIMITER = '.';
    const EXPECTED_CONST_VARS = '$SOME/CSRF/KEY$';

    protected $test;

    public function setUp()
    {
        $this->test = new Constants();
    }

    public function tearDown()
    {
        $this->test = null;
    }

    public function testConstantsDeclaration()
    {
        $this->assertNull(Constants::C1);
        $this->assertFalse(Constants::C2);
        $this->assertTrue(Constants::C3);
        $this->assertSame(Constants::C4, 10);
        $this->assertSame(Constants::C5, 10.25);
        $this->assertSame(Constants::C6, 'test');
        $this->assertSame(Constants::className, 'Test\Constants');
        $this->assertSame(Constants::STD_PROP_LIST, \ArrayObject::STD_PROP_LIST);
    }

    public function testConstantGetters()
    {
        $this->assertNull($this->test->getPropertyC1());
        $this->assertFalse($this->test->getPropertyC2());
        $this->assertTrue($this->test->getPropertyC3());
        $this->assertSame($this->test->getPropertyC4(), 10);
        $this->assertSame($this->test->getPropertyC5(), 10.25);
        $this->assertSame($this->test->getPropertyC6(), 'test');
        $this->assertSame($this->test->getPropertyC9(), 'some-value');
    }

    public function testConstantsRead()
    {
        $this->assertSame($this->test->testReadConstant(), ENT_QUOTES);
        $this->assertSame($this->test->testReadClassConstant1(), Constants::C4);
        $this->assertSame($this->test->testReadClassConstant2(), Constants::C4);
        $this->assertSame($this->test->testReadClassConstant3(), \Test\ConstantsParent::P4);
    }

    public function testEnvConstants()
    {
        $phpVersion = $this->test->testPHPVersionEnvConstant();
        $this->assertTrue(\is_string($phpVersion) && !empty($phpVersion));

        $phpVersion = $this->test->testPHPVersionEnvConstantInExpValue();
        $this->assertTrue(\is_string($phpVersion) && !empty($phpVersion));
    }

    public function testMagickConstants()
    {
        $this->assertSame($this->test->testNamespaceMagicConstant(), 'Test');
        $this->assertSame($this->test->testClassMagicConstant(), 'Test\Constants');

        $this->assertSame($this->test->testMethodMagicConstant(), 'Constants:testMethodMagicConstant');
        $this->assertSame($this->test->testFunctionMagicConstant(), 'testFunctionMagicConstant');

        $this->assertSame(ConstantsInterface::CLASSNAME, 'Test\Oo\ConstantsInterface');
        $this->assertSame(ConstantsInterface::NAMESPACENAME, 'Test\Oo');
    }

    /**
     * Test Constant declaration as String.
     *
     * @author Alexnder Andriiako <AlexNDR@phalconphp.com>
     *
     * @since 2017-08-09
     * @see https://github.com/phalcon/zephir/issues/1571
     */
    public function testStringDelimiterAsConstDoubleQuoted()
    {
        $this->assertSame($this->test->testStringDelimiterConstantDoubleQuoted(), self::EXPECTED_DOUBLE_DELIMITER);
    }

    public function testStringConstWithPhpVars()
    {
        $this->assertSame($this->test->testStringConstantWithVars(), self::EXPECTED_CONST_VARS);
    }

    public function testStringConstWithPhpVarsAssigned()
    {
        $this->assertSame($this->test->testStringPropertyWithVarsAssigned(), self::EXPECTED_CONST_VARS);
    }

    public function testStringConstWithPhpVarsGet()
    {
        $this->assertSame($this->test->testStringPropertyWithVarsGet(), self::EXPECTED_CONST_VARS);
    }
}
