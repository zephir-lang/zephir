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

// 2 params tests

assert($t->testTrim2Params('Hello World', "Hdle") == "o Wor");

assert($t->testLtrim2Params('Hello World', "Hdle") == "o World");

assert($t->testRtrim2Params('Hello World', "Hdle") == "Hello Wor");

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

assert($t->testSubstr('abcdef', 1, 3) == "bcd");
assert($t->testSubstr('abcdef', 0, 4) == "abcd" );
assert($t->testSubstr('abcdef', 0, 8) == "abcdef");
assert($t->testSubstr('abcdef', -1, 1) == "f");
assert($t->testSubstr('abcdef', -3, -1) == "de");
assert($t->testSubstr('abcdef', 2, -1) == "cde");

assert($t->testSubstr2('abcdef', -1) == "f");
assert($t->testSubstr2('abcdef', -2) == "ef");
assert($t->testSubstr2('abcdef', 2) == "cdef");

assert($t->testSubstr3('abcdef') == "f");
assert($t->testSubstr4('abcdef') == "abcde");

assert($t->testAddslashes("How's everybody") == addslashes("How's everybody"));
assert($t->testAddslashes('Are you "JOHN"?') == addslashes('Are you "JOHN"?'));
assert($t->testAddslashes("hello\0world") == addslashes("hello\0world"));

assert($t->testStripslashes(addslashes("How's everybody")) == "How's everybody");
assert($t->testStripslashes(addslashes('Are you "JOHN"?')) == 'Are you "JOHN"?');
assert($t->testStripslashes(addslashes("hello\0world")) == "hello\0world");

