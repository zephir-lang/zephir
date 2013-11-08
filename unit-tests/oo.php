<?php

$t = new Test\Oo();

assert($t->testInstance1() == new stdClass);
assert($t->testInstance2() == new Test\Oo\OoConstruct);
assert($t->testInstance3() == new Test\Oo\OoNoConstruct);

$obj4 = $t->testInstance4();
assert(is_object($obj4));
assert($obj4->getA() == "a");
assert($obj4->getB() == "b");

$obj5 = $t->testInstance5();
assert(is_object($obj5));
assert($obj5->getA() == "a");
assert($obj5->getB() == "b");

$obj6 = $t->testInstance6();
assert(is_object($obj6));
assert($obj6->getA() == 1);
assert($obj6->getB() == 2);

$obj7 = $t->testInstance7();
assert(is_object($obj7));
assert($obj7->getA() == false);
assert($obj7->getB() == true);

$obj8 = $t->testInstance8();
assert(is_object($obj8));
assert($obj8->getA() == 1.2);
assert($obj8->getB() == 7.30);

$obj9 = $t->testInstance9();
assert(is_object($obj9));
assert($obj9 instanceOf Test\Oo\OoDynamicA);

$obj10 = $t->testInstance10();
assert(is_object($obj10));
assert($obj10 instanceOf Test\Oo\OoDynamicB);
