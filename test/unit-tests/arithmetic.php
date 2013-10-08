<?php

$t = new Test\Arithmetic();

assert($t->intSum() === 3);
assert($t->int2Sum() === 3);
assert($t->intSumSimple() === 3);
assert($t->doubleSum() === 3.0);
assert($t->double2Sum() === 3.0);
assert($t->doubleSumSimple() === 3.0);
assert($t->doubleSum2Simple() === 3.0);
assert($t->boolSum() === true);
assert($t->bool2Sum() === true);
assert($t->bool3Sum() === true);
assert($t->boolSumSimple() === true);
assert($t->varSum() === 3);
assert($t->varSumSimple() === 3);
assert($t->intDoubleSum() === 3);
assert($t->intDoubleSumSimple() === 3);
assert($t->doubleIntSum() === 3.0);
assert($t->doubleIntSumSimple() === 3.0);
assert($t->varIntSum() === 3);
assert($t->intVarSum() === 3);
assert($t->intVarImplicitCastSum() == 3);
assert($t->intVarImplicitCast2Sum() == 3);
assert($t->complexSum() === 3.0);
assert($t->complex2Sum() === true);
assert($t->complex3Sum() === true);
assert($t->complex4Sum() === 2.0);
assert($t->complex5Sum() === 2);
assert($t->complex6Sum() === true);
assert($t->complex7Sum() === 3);
assert($t->complex9Sum() === 3);
assert($t->complex10Sum() === 3.0);
assert($t->complex11Sum() === 3.0);
assert($t->complex12Sum() === 3);
assert($t->complex13Sum() === 1);
assert($t->complex14Sum() === 2);
assert($t->complex15Sum() === true);
assert($t->complex17Sum() === 2.0);
assert($t->complex18Sum() === 4.0);
assert($t->complex19Sum() === 4.0);
assert($t->complex20Sum() === 8.0);
assert($t->complex21Sum() === 12.0);
assert($t->complex22Sum() === 21.0);

