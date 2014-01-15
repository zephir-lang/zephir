<?php

$t = new Test\Strings();

assert($t->testTrim(" hello ") == "hello");
assert($t->testTrim("hello ") == "hello");
assert($t->testTrim(" hello") == "hello");

assert($t->testLtrim(" hello ") == "hello ");
assert($t->testLtrim("hello ") == "hello ");
assert($t->testLtrim(" hello") == "hello");

assert($t->testRtrim(" hello ") == " hello");
assert($t->testRtrim("hello ") == "hello");
assert($t->testRtrim(" hello") == " hello");

assert($t->testStrpos("abcdef abcdef", "a") == 0);
assert($t->testStrposOffset("abcdef abcdef", "a", 1) == 7);

$pieces = array("a", "b", "c");
assert($t->testImplode(",", $pieces) == "a,b,c");


$pizza  = "piece1,piece2,piece3,piece4,piece5,piece6";
$ar1 = $t->testExplode(",", $pizza);
$ar2 = $t->testExplodeStr($pizza);

assert($ar1[0] == "piece1");
assert($ar1[2] == "piece3");

assert($ar2[0] == "piece1");
assert($ar2[2] == "piece3");

$ar3 = $t->testExplodeLimit($pizza, 3);
assert(count($ar3) == 3);
assert($ar3[2] == "piece3,piece4,piece5,piece6");
