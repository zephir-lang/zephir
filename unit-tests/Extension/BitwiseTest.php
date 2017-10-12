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

class BitwiseTest extends \PHPUnit_Framework_TestCase
{
    public function testBitwise()
    {
        $t = new \Test\Bitwise();

        // OR TESTS
        $this->assertSame($t->intAnd(), 0);
        $this->assertSame($t->int2And(), 0);
        $this->assertSame($t->intAndSimple(), 0);
        $this->assertSame($t->doubleAnd(), 0.0);
        $this->assertSame($t->double2And(), 0.0);
        $this->assertSame($t->doubleAndSimple(), 0.0);
        $this->assertSame($t->doubleAnd2Simple(), 0.0);
        $this->assertTrue($t->boolAnd());
        $this->assertFalse($t->bool2And());
        $this->assertFalse($t->bool3And());
        $this->assertFalse($t->boolAndSimple());
        $this->assertSame($t->varAnd(), 0);
        $this->assertSame($t->varAndSimple(), 0);
        $this->assertSame($t->intDoubleAnd(), 0);
        $this->assertSame($t->intDoubleAndSimple(), 0);
        $this->assertSame($t->doubleIntAnd(), 0.0);
        $this->assertSame($t->doubleIntAndSimple(), 0.0);
        $this->assertSame($t->varIntAnd(), 0);
        $this->assertSame($t->intVarAnd(), 0);
        $this->assertSame($t->intVarImplicitCastAnd(), 0);
        $this->assertSame($t->intVarImplicitCast2And(), 0);
        $this->assertSame($t->complexAnd(), 0.0);
        $this->assertTrue($t->complex2And());
        $this->assertTrue($t->complex3And());
        $this->assertSame($t->complex4And(), 1);
        $this->assertSame($t->complex5And(), 1);
        $this->assertTrue($t->complex6And());
        $this->assertSame($t->complex7And(), 0);
        $this->assertSame($t->complex9And(), 0);
        $this->assertSame($t->complex10And(), 0.0);
        $this->assertSame($t->complex11And(), 0.0);
        $this->assertSame($t->complex12And(), 0);
        $this->assertSame($t->complex13And(), 1);
        $this->assertSame($t->complex14And(), 1);
        $this->assertFalse($t->complex15And());
        $this->assertSame($t->complex17And(), 1.0);
        $this->assertSame($t->complex18And(), 1.0);
        $this->assertSame($t->complex19And(), 1.0);
        $this->assertSame($t->complex20And(), 1.0);
        $this->assertSame($t->complex21And(), 1.0);
        $this->assertSame($t->complex22And(), 0.0);
        $this->assertSame($t->complex23And(), 1.0);
        $this->assertSame($t->complex24And(), 0);

        // OR TESTS
        $this->assertSame($t->intOr(), 3);
        $this->assertSame($t->int2Or(), 3);
        $this->assertSame($t->intOrSimple(), 3);
        $this->assertSame($t->doubleOr(), 3.0);
        $this->assertSame($t->double2Or(), 3.0);
        $this->assertSame($t->doubleOrSimple(), 3.0);
        $this->assertSame($t->doubleOr2Simple(), 3.0);
        $this->assertTrue($t->boolOr());
        $this->assertTrue($t->bool2Or());
        $this->assertTrue($t->bool3Or());
        $this->assertTrue($t->boolOrSimple());
        $this->assertSame($t->varOr(), 3);
        $this->assertSame($t->varOrSimple(), 3);
        $this->assertSame($t->intDoubleOr(), 3);
        $this->assertSame($t->intDoubleOrSimple(), 3);
        $this->assertSame($t->doubleIntOr(), 3.0);
        $this->assertSame($t->doubleIntOrSimple(), 3.0);
        $this->assertSame($t->varIntOr(), 3);
        $this->assertSame($t->intVarOr(), 3);
        $this->assertSame($t->intVarImplicitCastOr(), 3);
        $this->assertSame($t->intVarImplicitCast2Or(), 3);
        $this->assertSame($t->complexOr(), 3.0);
        $this->assertTrue($t->complex2Or());
        $this->assertTrue($t->complex3Or());
        $this->assertSame($t->complex4Or(), 1);
        $this->assertSame($t->complex5Or(), 1);
        $this->assertTrue($t->complex6Or());
        $this->assertSame($t->complex7Or(), 3);
        $this->assertSame($t->complex9Or(), 3);
        $this->assertSame($t->complex10Or(), 3.0);
        $this->assertSame($t->complex11Or(), 3.0);
        $this->assertSame($t->complex12Or(), 3);
        $this->assertSame($t->complex13Or(), 1);
        $this->assertSame($t->complex14Or(), 1);
        $this->assertTrue($t->complex15Or());
        $this->assertSame($t->complex17Or(), 1.0);
        $this->assertSame($t->complex18Or(), 1.0);
        $this->assertSame($t->complex19Or(), 1.0);
        $this->assertSame($t->complex20Or(), 1.0);
        $this->assertSame($t->complex21Or(), 1.0);
        $this->assertSame($t->complex22Or(), 3.0);
        $this->assertSame($t->complex23Or(), 1.0);
        $this->assertSame($t->complex24Or(), 1.0);

        // shiftleft
        $this->assertSame($t->intShiftLeft(), 4);
        $this->assertSame($t->int2ShiftLeft(), 4);
        $this->assertSame($t->intShiftLeftSimple(), 4);

        // shiftright
        $this->assertSame($t->intShiftRight(), 0);
        $this->assertSame($t->int2ShiftRight(), 0);
        $this->assertSame($t->intShiftRightSimple(), 0);

        // Bitwise NOT
        $this->assertSame($t->testBitwiseNot(666), -667);
        $this->assertSame($t->testBitwiseAndNot(5, 4), 1);

        /**
         * Bitwise XOR
         * @issue 1581
         */
        $this->assertSame(123 ^ 321, $t->testbitwiseXor());
    }
}
