<?php

$t = new Test\InstanceOff();

assert($t->testInstanceOf1() === true);
assert($t->testInstanceOf2() === true);
assert($t->testInstanceOf3() === false);