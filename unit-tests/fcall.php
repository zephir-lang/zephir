<?php

$t = new Test\Fcall();

assert($t->testCall1() === 0);
assert($t->testCall1FromVar() === 2);

assert($t->testStrtokFalse() === false);
assert($t->testStrtokVarBySlash('test') === 'test');

assert($t->testFunctionGetArgs(true, false) === array(true, false));
assert($t->testFunctionGetArgs(1025, false) === array(1025, false));
assert($t->testFunctionGetArgs(false, 1234) === array(false, 1234));
assert($t->testFunctionGetArgs(array(1, 2, 3), false) === array(array(1, 2, 3), false));