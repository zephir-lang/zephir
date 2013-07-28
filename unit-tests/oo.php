<?php

$t = new Test\Oo();

assert($t->testInstance1() == new stdClass);
assert($t->testInstance2() == new Test\Oo\OoConstruct);
