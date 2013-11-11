<?php

assert(Test\TestInterface::C1 === null);
assert(Test\TestInterface::C2 === false);
assert(Test\TestInterface::C3 === true);
assert(Test\TestInterface::C4 === 10);
assert(Test\TestInterface::C5 === 10.25);
assert(Test\TestInterface::C6 === "test");

$t = new Test\ConstantsInterface();

assert(is_object($t));
assert($t->testReadInterfaceConstant1() === null);
assert($t->testReadInterfaceConstant2() === false);
assert($t->testReadInterfaceConstant3() === true);
assert($t->testReadInterfaceConstant4() === 10);
assert($t->testReadInterfaceConstant5() === 10.25);
assert($t->testReadInterfaceConstant6() === "test");
