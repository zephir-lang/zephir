<?php

$t = new Test\Mcall();

assert($t->testCall1() === "hello public");
assert($t->testCall2() === "hello protected");
assert($t->testCall3() === "hello private");

assert($t->testCall4(2, 3) === 5);
assert($t->testCall5(6, 7) === 13);
assert($t->testCall6(4, 5) === 9);

assert($t->testCall7(2, 3) === 10);
assert($t->testCall8(6, 7) === 26);
assert($t->testCall9(4, 5) === 18);
