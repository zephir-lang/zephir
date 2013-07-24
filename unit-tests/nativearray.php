<?php

$t = new Test\NativeArray();

assert($t->testArray1() == array());
assert($t->testArray2() == array(1, 2, 3));
assert($t->testArray3() == array(1.1, 2.2, 3.3));
assert($t->testArray4() == array(false, true, false));
assert($t->testArray5() == array(null, null, null));
assert($t->testArray6() == array("x", "y", "z"));
assert($t->testArray7() == array(1, 2, 3));