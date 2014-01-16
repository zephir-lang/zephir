<?php

$t = new Test\Typeoff();

assert($t->testNativeStringFalse() == false);
assert($t->testNativeStringTrue() == true);

assert($t->testNativeIntFalse() == false);
assert($t->testNativeIntTrue() == true);

assert($t->testNativeDoubleTrue() == true);

assert($t->testNativeBoolFalse() == false);
assert($t->testNativeBoolTrue() == true);

assert($t->testArrayFalse() == false);
assert($t->testArrayTrue() == true);

assert($t->testClassPropertyAccess() == true);