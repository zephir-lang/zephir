<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class ArithmeticTest extends \PHPUnit_Framework_TestCase
{
    protected $class;

    public function setUp()
    {
        $this->class = new \Test\Arithmetic();
    }

    public function testIntSum()
    {
        $this->assertSame($this->class->intSum(), 3);
        $this->assertSame($this->class->int2Sum(), 3);
        $this->assertSame($this->class->intSumSimple(), 3);
    }

    public function testDoubleSum()
    {
        $this->assertSame($this->class->doubleSum(), 3.0);
        $this->assertSame($this->class->double2Sum(), 3.0);
        $this->assertSame($this->class->doubleSumSimple(), 3.0);
        $this->assertSame($this->class->doubleSum2Simple(), 3.0);
    }

    public function testSummTest()
    {
        $this->assertSame($this->class->varSum(), 3);
        $this->assertSame($this->class->varSumSimple(), 3);
        $this->assertSame($this->class->intDoubleSum(), 3);
        $this->assertSame($this->class->intDoubleSumSimple(), 3);
        $this->assertSame($this->class->doubleIntSum(), 3.0);
        $this->assertSame($this->class->doubleIntSumSimple(), 3.0);
        $this->assertSame($this->class->varIntSum(), 3);
        $this->assertSame($this->class->intVarSum(), 3);
        $this->assertSame($this->class->intVarImplicitCastSum(), 3);
        $this->assertSame($this->class->intVarImplicitCast2Sum(), 3);
        $this->assertSame($this->class->complexSum(), 3.0);
        $this->assertTrue($this->class->complex2Sum());
        $this->assertTrue($this->class->complex3Sum());

        $this->assertSame(2.0, $this->class->complex4Sum());
        $this->assertSame(2, $this->class->complex5Sum());
        $this->assertTrue($this->class->complex6Sum());
        $this->assertSame(3, $this->class->complex7Sum());
        $this->assertSame(3, $this->class->complex9Sum());
        $this->assertSame($this->class->complex10Sum(), 3.0);
        $this->assertSame($this->class->complex11Sum(), 3.0);
        $this->assertSame($this->class->complex12Sum(), 3);
        $this->assertSame($this->class->complex13Sum(), 1);
        $this->assertSame($this->class->complex14Sum(), 2);
        $this->assertTrue($this->class->complex15Sum());
        $this->assertSame($this->class->complex17Sum(), 2.0);
        $this->assertSame($this->class->complex18Sum(), 4.0);
        $this->assertSame($this->class->complex19Sum(), 4.0);
        $this->assertSame($this->class->complex20Sum(), 8.0);
        $this->assertSame($this->class->complex21Sum(), 12.0);
        $this->assertSame($this->class->complex22Sum(), 21.0);
        $this->assertSame($this->class->complex23Sum(), 2.0);
        $this->assertSame($this->class->complex24Sum(), 3.0);
    }

    public function testBoolSum()
    {
        $this->assertTrue($this->class->boolSum());
        $this->assertTrue($this->class->bool2Sum());
        $this->assertTrue($this->class->bool3Sum());
        $this->assertTrue($this->class->boolSumSimple());
    }

    public function testVarLess()
    {
        $this->assertTrue($this->class->less1());
        $this->assertFalse($this->class->less2());

        /**
         * X < 5
         */
        $this->assertFalse($this->class->less3(100));
        $this->assertFalse($this->class->less3(6));
        $this->assertFalse($this->class->less3(5));
        $this->assertTrue($this->class->less3(4));

        /**
         * X < 5.0
         */
        $this->assertFalse($this->class->less4(100));
        $this->assertFalse($this->class->less4(6));
        $this->assertFalse($this->class->less4(5));
        $this->assertTrue($this->class->less4(4));
    }

    public function testVarGreater()
    {
        $this->assertFalse($this->class->greater1());
        $this->assertTrue($this->class->greater2());

        /**
         * X > 5
         */
        $this->assertTrue($this->class->greater3(100));
        $this->assertTrue($this->class->greater3(6));
        $this->assertFalse($this->class->greater3(5));
        $this->assertFalse($this->class->greater3(4));

        /**
         * X > 5.0
         */
        $this->assertTrue($this->class->greater4(100));
        $this->assertTrue($this->class->greater4(6));
        $this->assertFalse($this->class->greater4(5));
        $this->assertFalse($this->class->greater4(4));
    }

    public function testAddSum()
    {
        $this->assertSame($this->class->addSum1(), 10);
        $this->assertSame($this->class->addSum2(), 0);
        $this->assertSame($this->class->addSum2b(), 5);
        $this->assertSame($this->class->addSum3(), 1);
        $this->assertSame($this->class->addSum4(), 0);
        $this->assertSame($this->class->addSum5(), 1);
        $this->assertSame($this->class->addSum6(), 1);
        $this->assertSame($this->class->addSum7(), 10.0);
        $this->assertSame($this->class->addSum8(), 0.0);
        $this->assertSame($this->class->addSum8b(), 6.3);

        $this->assertSame($this->class->addSum9(), 1.0);
        $this->assertSame($this->class->addSum10(), 0.0);
        $this->assertSame($this->class->addSum11(), 1.0);
        $this->assertSame($this->class->addSum12(), 10);
        $this->assertSame($this->class->addSum13(), 1);

        $this->assertSame($this->class->addSum14(), 0);
        $this->assertSame($this->class->addSum15(), 1);
        $this->assertSame($this->class->addSum16(), 10.0);
        $this->assertSame($this->class->addSum17(), 1.0);
        $this->assertSame($this->class->addSum18(), 0.0);
        $this->assertSame($this->class->addSum19(), 1.0);
        $this->assertSame($this->class->addSum20(), 1);

        $this->assertSame($this->class->addSum21(), 1.0);
        $this->assertSame($this->class->addSum22(), 1);
        $this->assertSame($this->class->addSum23(), 1 + (1 << 10));
        $this->assertSame($this->class->addSum24(1), 1 + (1 << 10));
    }

    public function testSubTest()
    {
        $this->assertSame($this->class->intSub(), -1);
        $this->assertSame($this->class->intSub2(), -1);
        $this->assertSame($this->class->intLetSub(), -1);
        $this->assertSame($this->class->intSubSimple(), -1);

        $this->assertTrue($this->class->boolSub());
        $this->assertTrue($this->class->bool2Sub());
        $this->assertFalse($this->class->bool3Sub()); // true - false
        $this->assertTrue($this->class->bool4Sub());  // true - true
        $this->assertTrue($this->class->boolSubSimple());
        $this->assertSame($this->class->doubleSub(), -1.0);
        $this->assertSame($this->class->double2Sub(), -1.0);
        $this->assertSame($this->class->doubleSubSimple(), -1.0);
        $this->assertSame($this->class->doubleSub2Simple(), -1.0);
        $this->assertSame($this->class->varSub(), -1);
        $this->assertSame($this->class->varSubSimple(), -1);
        $this->assertSame($this->class->intDoubleSub(), -1);
        $this->assertSame($this->class->intDoubleSubSimple(), -1);
        $this->assertSame($this->class->doubleIntSub(), -1.0);
        $this->assertSame($this->class->doubleIntSubSimple(), -1.0);
        $this->assertSame($this->class->varIntSub(), -1);
        $this->assertSame($this->class->intVarSub(), 1);
        $this->assertSame($this->class->intVarImplicitCastSub(), 1);
        $this->assertSame($this->class->intVarImplicitCast2Sub(), 1);
        $this->assertSame($this->class->complexSub(), 1.0);
        $this->assertTrue($this->class->complex2Sub());
        $this->assertTrue($this->class->complex3Sub());
        $this->assertSame(0.0, $this->class->complex4Sub());
        $this->assertSame($this->class->complex5Sub(), 0);
        $this->assertSame($this->class->complex6Sub(), true);
        $this->assertSame($this->class->complex7Sub(), -1);
        $this->assertSame($this->class->complex9Sub(), 1);
        $this->assertSame($this->class->complex10Sub(), 1.0);
        $this->assertSame($this->class->complex11Sub(), 1.0);
        $this->assertSame($this->class->complex12Sub(), 1);
        $this->assertSame($this->class->complex13Sub(), 1);
        $this->assertSame($this->class->complex14Sub(), 0);
        $this->assertSame($this->class->complex15Sub(), true);
        $this->assertSame($this->class->complex17Sub(), 0.0);
        $this->assertSame($this->class->complex18Sub(), 0.0);
        $this->assertSame($this->class->complex19Sub(), 0.0);
        $this->assertSame($this->class->complex20Sub(), 0.0);
        $this->assertSame($this->class->complex21Sub(), -2.0);
        $this->assertSame($this->class->complex22Sub(), 3.0);
        $this->assertSame($this->class->complex23Sub(), 0.0);
        $this->assertSame($this->class->complex24Sub(), -1.0);
        $this->assertSame($this->class->sub1(), -10);
        $this->assertSame($this->class->sub2(), 0);
        $this->assertSame($this->class->sub2b(), 5);
        $this->assertSame($this->class->sub3(), -1);
        $this->assertSame($this->class->sub4(), 0);
        $this->assertSame($this->class->sub5(), -1);
        $this->assertSame($this->class->sub6(), -1);
        $this->assertSame($this->class->sub7(), -10.0);
        $this->assertSame($this->class->sub8(), 0.0);
        $this->assertSame($this->class->sub8b(), 6.3);
        $this->assertSame($this->class->sub9(), -1.0);
        $this->assertSame($this->class->sub10(), 0.0);
        $this->assertSame($this->class->sub11(), -1.0);
        $this->assertSame($this->class->sub12(), -10);
        $this->assertSame($this->class->sub13(), -1);
        $this->assertSame($this->class->sub14(), 0);
        $this->assertSame($this->class->sub15(), -1);
        $this->assertSame($this->class->sub16(), -10.0);
        $this->assertSame($this->class->sub17(), -1.0);
        $this->assertSame($this->class->sub18(), 0.0);
        $this->assertSame($this->class->sub19(), -1.0);
        $this->assertSame($this->class->sub20(), -1);
        $this->assertSame($this->class->sub21(), -1.0);
        $this->assertSame($this->class->sub22(), -1);
        $this->assertSame($this->class->sub23(), 1 - (1 << 10));
        $this->assertSame($this->class->sub24(1), 1 - (1 << 10));
    }

    public function testMul()
    {
        $this->assertSame($this->class->mul1(), 5);
        $this->assertSame($this->class->mul2(5), 25);
        $this->assertSame($this->class->mul3(), 1 * (1 << 10));
    }

    public function testDiv()
    {
        $this->assertSame(24.75, $this->class->div1());
        $this->assertSame(24.75, $this->class->div2());
    }

    public function testLetStatement()
    {
        $this->assertSame($this->class->letStatementIntMinus(), -1);
        $this->assertSame($this->class->letStatementVarMinus(1), -1);
        $this->assertSame($this->class->letStatementVarMinus(-1), 1);
        $this->assertSame($this->class->letStatementVarMinus(true), -true);
        $this->assertSame($this->class->letStatementVarMinus(false), -false);
        $this->assertSame($this->class->letStatementVarMinus(""), @-"");
        $this->assertTrue($this->class->letStatementBoolMinus(-1)); // it is zend_bool not zend_var
        $this->assertTrue($this->class->letStatementBoolMinus(1));
        $this->assertTrue($this->class->letStatementBoolMinus(true));
        $this->assertFalse($this->class->letStatementBoolMinus(false));
        $this->assertFalse($this->class->letStatementBoolMinus(0));
    }
}
