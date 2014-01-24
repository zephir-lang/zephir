<?php

$t = new Test\Unsettest();

$testVar = array(1, 2, 3, 4, 5, 6);
assert($t->testUnsetFromArray($testVar) == array(1 => 2, 3, 4, 5, 6));
assert($t->testUnsetFromArrayByIndexVar($testVar, 0) == array(1 => 2, 3, 4, 5, 6));

assert($t->has('testVar') === false);
$t->addValueToProperty('testVar', $testVar);
assert($t->has('testVar') === true);
$t->testUnsetValueFromProperty('testVar');
assert($t->has('testVar') === false);