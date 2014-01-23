<?php

$t = new Test\Bitwise();

// OR TESTS

assert($t->intAnd() === 0);
assert($t->int2And() === 0);
assert($t->intAndSimple() === 0);
assert($t->doubleAnd() === 0.0);
assert($t->double2And() === 0.0);
assert($t->doubleAndSimple() === 0.0);
assert($t->doubleAnd2Simple() === 0.0);
assert($t->boolAnd() === true);
assert($t->bool2And() === false);
assert($t->bool3And() === false);
assert($t->boolAndSimple() === false);
assert($t->varAnd() === 0);
assert($t->varAndSimple() === 0);
assert($t->intDoubleAnd() === 0);
assert($t->intDoubleAndSimple() === 0);
assert($t->doubleIntAnd() === 0.0);
assert($t->doubleIntAndSimple() === 0.0);
assert($t->varIntAnd() === 0);
assert($t->intVarAnd() === 0);
assert($t->intVarImplicitCastAnd() === 0);
assert($t->intVarImplicitCast2And() === 0);
assert($t->complexAnd() === 0.0);
assert($t->complex2And() === true);
assert($t->complex3And() === true);
assert($t->complex4And() === 1);
assert($t->complex5And() === 1);
assert($t->complex6And() === true);
assert($t->complex7And() === 0);
assert($t->complex9And() === 0);
assert($t->complex10And() === 0.0);
assert($t->complex11And() === 0.0);
assert($t->complex12And() === 0);
assert($t->complex13And() === 1);
assert($t->complex14And() === 1);
assert($t->complex15And() === false);
assert($t->complex17And() === 1.0);
assert($t->complex18And() === 1.0);
assert($t->complex19And() === 1.0);
assert($t->complex20And() === 1.0);
assert($t->complex21And() === 1.0);
assert($t->complex22And() === 0.0);
assert($t->complex23And() === 1.0);
assert($t->complex24And() === 0.0);

// OR TESTS

assert($t->intOr() === 3);
assert($t->int2Or() === 3);
assert($t->intOrSimple() === 3);
assert($t->doubleOr() === 3.0);
assert($t->double2Or() === 3.0);
assert($t->doubleOrSimple() === 3.0);
assert($t->doubleOr2Simple() === 3.0);
assert($t->boolOr() === true);
assert($t->bool2Or() === true);
assert($t->bool3Or() === true);
assert($t->boolOrSimple() === true);
assert($t->varOr() === 3);
assert($t->varOrSimple() === 3);
assert($t->intDoubleOr() === 3);
assert($t->intDoubleOrSimple() === 3);
assert($t->doubleIntOr() === 3.0);
assert($t->doubleIntOrSimple() === 3.0);
assert($t->varIntOr() === 3);
assert($t->intVarOr() === 3);
assert($t->intVarImplicitCastOr() === 3);
assert($t->intVarImplicitCast2Or() === 3);
assert($t->complexOr() === 3.0);
assert($t->complex2Or() === true);
assert($t->complex3Or() === true);
assert($t->complex4Or() === 1);
assert($t->complex5Or() === 1);
assert($t->complex6Or() === true);
assert($t->complex7Or() === 3);
assert($t->complex9Or() === 3);
assert($t->complex10Or() === 3.0);
assert($t->complex11Or() === 3.0);
assert($t->complex12Or() === 3);
assert($t->complex13Or() === 1);
assert($t->complex14Or() === 1);
assert($t->complex15Or() === true);
assert($t->complex17Or() === 1.0);
assert($t->complex18Or() === 1.0);
assert($t->complex19Or() === 1.0);
assert($t->complex20Or() === 1.0);
assert($t->complex21Or() === 1.0);
assert($t->complex22Or() === 3.0);
assert($t->complex23Or() === 1.0);
assert($t->complex24Or() === 1.0);

// shiftleft
assert($t->intShiftLeft() === 4);
assert($t->int2ShiftLeft() === 4);
assert($t->intShiftLeftSimple() === 4);

// shiftright
assert($t->intShiftRight() === 0);
assert($t->int2ShiftRight() === 0);
assert($t->intShiftRightSimple() === 0);

