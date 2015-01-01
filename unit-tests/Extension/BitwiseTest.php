<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class BitwiseTest extends \PHPUnit_Framework_TestCase
{
    public function testBitwise()
    {
        $t = new \Test\Bitwise();

        // OR TESTS
        $this->assertTrue($t->intAnd() === 0);
        $this->assertTrue($t->int2And() === 0);
        $this->assertTrue($t->intAndSimple() === 0);
        $this->assertTrue($t->doubleAnd() === 0.0);
        $this->assertTrue($t->double2And() === 0.0);
        $this->assertTrue($t->doubleAndSimple() === 0.0);
        $this->assertTrue($t->doubleAnd2Simple() === 0.0);
        $this->assertTrue($t->boolAnd() === true);
        $this->assertTrue($t->bool2And() === false);
        $this->assertTrue($t->bool3And() === false);
        $this->assertTrue($t->boolAndSimple() === false);
        $this->assertTrue($t->varAnd() === 0);
        $this->assertTrue($t->varAndSimple() === 0);
        $this->assertTrue($t->intDoubleAnd() === 0);
        $this->assertTrue($t->intDoubleAndSimple() === 0);
        $this->assertTrue($t->doubleIntAnd() === 0.0);
        $this->assertTrue($t->doubleIntAndSimple() === 0.0);
        $this->assertTrue($t->varIntAnd() === 0);
        $this->assertTrue($t->intVarAnd() === 0);
        $this->assertTrue($t->intVarImplicitCastAnd() === 0);
        $this->assertTrue($t->intVarImplicitCast2And() === 0);
        $this->assertTrue($t->complexAnd() === 0.0);
        $this->assertTrue($t->complex2And() === true);
        $this->assertTrue($t->complex3And() === true);
        $this->assertTrue($t->complex4And() === 1);
        $this->assertTrue($t->complex5And() === 1);
        $this->assertTrue($t->complex6And() === true);
        $this->assertTrue($t->complex7And() === 0);
        $this->assertTrue($t->complex9And() === 0);
        $this->assertTrue($t->complex10And() === 0.0);
        $this->assertTrue($t->complex11And() === 0.0);
        $this->assertTrue($t->complex12And() === 0);
        $this->assertTrue($t->complex13And() === 1);
        $this->assertTrue($t->complex14And() === 1);
        $this->assertTrue($t->complex15And() === false);
        $this->assertTrue($t->complex17And() === 1.0);
        $this->assertTrue($t->complex18And() === 1.0);
        $this->assertTrue($t->complex19And() === 1.0);
        $this->assertTrue($t->complex20And() === 1.0);
        $this->assertTrue($t->complex21And() === 1.0);
        $this->assertTrue($t->complex22And() === 0.0);
        $this->assertTrue($t->complex23And() === 1.0);
        $this->assertTrue($t->complex24And() === 0);

        // OR TESTS
        $this->assertTrue($t->intOr() === 3);
        $this->assertTrue($t->int2Or() === 3);
        $this->assertTrue($t->intOrSimple() === 3);
        $this->assertTrue($t->doubleOr() === 3.0);
        $this->assertTrue($t->double2Or() === 3.0);
        $this->assertTrue($t->doubleOrSimple() === 3.0);
        $this->assertTrue($t->doubleOr2Simple() === 3.0);
        $this->assertTrue($t->boolOr() === true);
        $this->assertTrue($t->bool2Or() === true);
        $this->assertTrue($t->bool3Or() === true);
        $this->assertTrue($t->boolOrSimple() === true);
        $this->assertTrue($t->varOr() === 3);
        $this->assertTrue($t->varOrSimple() === 3);
        $this->assertTrue($t->intDoubleOr() === 3);
        $this->assertTrue($t->intDoubleOrSimple() === 3);
        $this->assertTrue($t->doubleIntOr() === 3.0);
        $this->assertTrue($t->doubleIntOrSimple() === 3.0);
        $this->assertTrue($t->varIntOr() === 3);
        $this->assertTrue($t->intVarOr() === 3);
        $this->assertTrue($t->intVarImplicitCastOr() === 3);
        $this->assertTrue($t->intVarImplicitCast2Or() === 3);
        $this->assertTrue($t->complexOr() === 3.0);
        $this->assertTrue($t->complex2Or() === true);
        $this->assertTrue($t->complex3Or() === true);
        $this->assertTrue($t->complex4Or() === 1);
        $this->assertTrue($t->complex5Or() === 1);
        $this->assertTrue($t->complex6Or() === true);
        $this->assertTrue($t->complex7Or() === 3);
        $this->assertTrue($t->complex9Or() === 3);
        $this->assertTrue($t->complex10Or() === 3.0);
        $this->assertTrue($t->complex11Or() === 3.0);
        $this->assertTrue($t->complex12Or() === 3);
        $this->assertTrue($t->complex13Or() === 1);
        $this->assertTrue($t->complex14Or() === 1);
        $this->assertTrue($t->complex15Or() === true);
        $this->assertTrue($t->complex17Or() === 1.0);
        $this->assertTrue($t->complex18Or() === 1.0);
        $this->assertTrue($t->complex19Or() === 1.0);
        $this->assertTrue($t->complex20Or() === 1.0);
        $this->assertTrue($t->complex21Or() === 1.0);
        $this->assertTrue($t->complex22Or() === 3.0);
        $this->assertTrue($t->complex23Or() === 1.0);
        $this->assertTrue($t->complex24Or() === 1.0);

        // shiftleft
        $this->assertTrue($t->intShiftLeft() === 4);
        $this->assertTrue($t->int2ShiftLeft() === 4);
        $this->assertTrue($t->intShiftLeftSimple() === 4);

        // shiftright
        $this->assertTrue($t->intShiftRight() === 0);
        $this->assertTrue($t->int2ShiftRight() === 0);
        $this->assertTrue($t->intShiftRightSimple() === 0);
    }
}
