<?php

$t = new Test\Constants();

assert(Test\Constants::C1 === null);
assert(Test\Constants::C2 === false);
assert(Test\Constants::C3 === true);
assert(Test\Constants::C4 === 10);
assert(Test\Constants::C5 === 10.25);
assert(Test\Constants::C6 === "test");

assert($t->testReadConstant() === 3);
assert($t->testReadClassConstant1() === 10);
assert($t->testReadClassConstant2() === 10);
assert($t->testReadClassConstant3() === 10);
