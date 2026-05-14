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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Bitwise;

final class BitwiseTest extends TestCase
{
    public function testBitwise(): void
    {
        $test = new Bitwise();

        // OR TESTS
        $this->assertSame($test->intAnd(), 0);
        $this->assertSame($test->int2And(), 0);
        $this->assertSame($test->intAndSimple(), 0);
        $this->assertSame($test->doubleAnd(), 0.0);
        $this->assertSame($test->double2And(), 0.0);
        $this->assertSame($test->doubleAndSimple(), 0.0);
        $this->assertSame($test->doubleAnd2Simple(), 0.0);
        $this->assertTrue($test->boolAnd());
        $this->assertFalse($test->bool2And());
        $this->assertFalse($test->bool3And());
        $this->assertFalse($test->boolAndSimple());
        $this->assertSame($test->varAnd(), 0);
        $this->assertSame($test->varAndSimple(), 0);
        $this->assertSame($test->intDoubleAnd(), 0);
        $this->assertSame($test->intDoubleAndSimple(), 0);
        $this->assertSame($test->doubleIntAnd(), 0.0);
        $this->assertSame($test->doubleIntAndSimple(), 0.0);
        $this->assertSame($test->varIntAnd(), 0);
        $this->assertSame($test->intVarAnd(), 0);
        $this->assertSame($test->intVarImplicitCastAnd(), 0);
        $this->assertSame($test->intVarImplicitCast2And(), 0);
        $this->assertSame($test->complexAnd(), 0.0);
        $this->assertTrue($test->complex2And());
        $this->assertTrue($test->complex3And());
        $this->assertSame($test->complex4And(), 1);
        $this->assertSame($test->complex5And(), 1);
        $this->assertTrue($test->complex6And());
        $this->assertSame($test->complex7And(), 0);
        $this->assertSame($test->complex9And(), 0);
        $this->assertSame($test->complex10And(), 0.0);
        $this->assertSame($test->complex11And(), 0.0);
        $this->assertSame($test->complex12And(), 0);
        $this->assertSame($test->complex13And(), 1);
        $this->assertSame($test->complex14And(), 1);
        $this->assertFalse($test->complex15And());
        $this->assertSame($test->complex17And(), 1.0);
        $this->assertSame($test->complex18And(), 1.0);
        $this->assertSame($test->complex19And(), 1.0);
        $this->assertSame($test->complex20And(), 1.0);
        $this->assertSame($test->complex21And(), 1.0);
        $this->assertSame($test->complex22And(), 0.0);
        $this->assertSame($test->complex23And(), 1.0);
        $this->assertSame($test->complex24And(), 0);

        // OR TESTS
        $this->assertSame($test->intOr(), 3);
        $this->assertSame($test->int2Or(), 3);
        $this->assertSame($test->intOrSimple(), 3);
        $this->assertSame($test->doubleOr(), 3.0);
        $this->assertSame($test->double2Or(), 3.0);
        $this->assertSame($test->doubleOrSimple(), 3.0);
        $this->assertSame($test->doubleOr2Simple(), 3.0);
        $this->assertTrue($test->boolOr());
        $this->assertTrue($test->bool2Or());
        $this->assertTrue($test->bool3Or());
        $this->assertTrue($test->boolOrSimple());
        $this->assertSame($test->varOr(), 3);
        $this->assertSame($test->varOrSimple(), 3);
        $this->assertSame($test->intDoubleOr(), 3);
        $this->assertSame($test->intDoubleOrSimple(), 3);
        $this->assertSame($test->doubleIntOr(), 3.0);
        $this->assertSame($test->doubleIntOrSimple(), 3.0);
        $this->assertSame($test->varIntOr(), 3);
        $this->assertSame($test->intVarOr(), 3);
        $this->assertSame($test->intVarImplicitCastOr(), 3);
        $this->assertSame($test->intVarImplicitCast2Or(), 3);
        $this->assertSame($test->complexOr(), 3.0);
        $this->assertTrue($test->complex2Or());
        $this->assertTrue($test->complex3Or());
        $this->assertSame($test->complex4Or(), 1);
        $this->assertSame($test->complex5Or(), 1);
        $this->assertTrue($test->complex6Or());
        $this->assertSame($test->complex7Or(), 3);
        $this->assertSame($test->complex9Or(), 3);
        $this->assertSame($test->complex10Or(), 3.0);
        $this->assertSame($test->complex11Or(), 3.0);
        $this->assertSame($test->complex12Or(), 3);
        $this->assertSame($test->complex13Or(), 1);
        $this->assertSame($test->complex14Or(), 1);
        $this->assertTrue($test->complex15Or());
        $this->assertSame($test->complex17Or(), 1.0);
        $this->assertSame($test->complex18Or(), 1.0);
        $this->assertSame($test->complex19Or(), 1.0);
        $this->assertSame($test->complex20Or(), 1.0);
        $this->assertSame($test->complex21Or(), 1.0);
        $this->assertSame($test->complex22Or(), 3.0);
        $this->assertSame($test->complex23Or(), 1.0);
        $this->assertSame($test->complex24Or(), 1.0);

        // shiftleft
        $this->assertSame($test->intShiftLeft(), 4);
        $this->assertSame($test->int2ShiftLeft(), 4);
        $this->assertSame($test->intShiftLeftSimple(), 4);

        // shiftright
        $this->assertSame($test->intShiftRight(), 0);
        $this->assertSame($test->int2ShiftRight(), 0);
        $this->assertSame($test->intShiftRightSimple(), 0);

        // Bitwise NOT
        $this->assertSame($test->testBitwiseNot(666), -667);
        $this->assertSame($test->testBitwiseAndNot(5, 4), 1);

        /**
         * Bitwise XOR
         *
         * @issue https://github.com/zephir-lang/zephir/issues/1581
         */
        $this->assertSame(123 ^ 321, $test->testbitwiseXor());
    }
}
