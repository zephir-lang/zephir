<?php
$t = new Test\Constants();

assert(Test\Constants::C1 === null);
assert(Test\Constants::C2 === false);
assert(Test\Constants::C3 === true);
assert(Test\Constants::C4 === 10);
assert(Test\Constants::C5 === 10.25);
assert(Test\Constants::C6 === "test");

assert($t->testReadConstant() === ENT_QUOTES);
assert($t->testReadClassConstant1() === Test\Constants::C4);
assert($t->testReadClassConstant2() === Test\Constants::C4);
assert($t->testReadClassConstant3() === Test\ConstantsParent::P4);

$phpVersion = $t->testPHPVersionEnvConstant();
assert(is_string($phpVersion) && !empty($phpVersion));

assert($t->testClassMagicConstant() == 'Constants');
assert($t->testMethodMagicConstant() == 'testMethodMagicConstant');
