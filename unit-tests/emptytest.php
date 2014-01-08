<?php

$t = new Test\EmptyTest();

assert($t->testDynamicVarArrayEmpty() == true);
assert($t->testDynamicVarArrayNotEmpty() == false);
assert($t->testEmptyString() == true);
assert($t->testNotEmptyString() == false);