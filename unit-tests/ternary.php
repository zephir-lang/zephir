<?php

$t = new Test\Ternary();

assert($t->testTernary1() === 101);
assert($t->testTernary2(true) === "foo");
assert($t->testTernary2(false) === "bar");
assert($t->testTernaryComplex1(array(), "") === 101);
assert($t->testTernaryComplex2(array(), "") === 106);
assert($t->testTernaryComplex3("") === "boolean");
