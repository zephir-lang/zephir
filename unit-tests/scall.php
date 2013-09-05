<?php

$t = new Test\Scall();

assert($t->testMethod1() === "hello public");
assert($t::testMethod1() === "hello public");
assert(Test\Scall::testMethod1() === "hello public");

assert($t->testCall1() === "hello public");
assert($t->testCall2() === "hello protected");
assert($t->testCall3() === "hello private");

assert($t->testCall4(2, 3) === 5);
assert($t->testCall5(6, 7) === 13);
assert($t->testCall6(4, 5) === 9);

assert($t->testCall7() === "hello public");
assert($t->testCall8() === "hello protected");
assert($t->testCall9() === "hello private");

assert($t->testCall10(2, 3) === 5);
assert($t->testCall11(6, 7) === 13);
assert($t->testCall12(4, 5) === 9);

assert($t->testCall13() == "hello parent public");
assert($t->testCall14() == "hello parent protected");