<?php

$t = new Test\Pregmatch();

assert($t->testWithoutReturnAndMatches());

assert($t->testWithoutReturns() == array('def'));

assert($t->testWithoutMatches());

assert($t->testPregMatchAll());

assert($t->testPregMatchFallback());


// more tests from php5 ext/pcre
$string = 'Hello, world. [*], this is \ a string';
$match1 = null;
assert($t->testPregMatch3Params('/^[hH]ello,\s/', $string, $match1)); //finds "Hello, "
assert($match1[0] == "Hello, ");

$match2 = null;
assert($t->testPregMatch4Params('/l^o,\s\w{5}/', $string, $match2, PREG_OFFSET_CAPTURE) == false); // tries to find "lo, world" at start of string
assert(count($match2) == 0);

$match3 = null;
assert($t->testPregMatch3Params('/\[\*\],\s(.*)/', $string, $match3)); //finds "[*], this is \ a string";
assert(count($match3) == 2);

$match4 = null;
assert($t->testPregMatch5Params('@\w{4}\s\w{2}\s\\\(?:\s.*)@', $string, $match4, PREG_OFFSET_CAPTURE, 14)); //finds "this is \ a string" (with non-capturing parentheses)
assert($match4[0][0] == 'this is \ a string');

$match5 = null;
assert($t->testPregMatch3Params('/hello world/', $string, $match5) == false); //tries to find "hello world" (should be Hello, world)
assert(count($match5) == 0);

$string2 = "My\nName\nIs\nStrange";
$match6 = null;
assert($t->testPregMatch3Params("/M(.*)/", $string2, $match6));
assert(count($match6) == 2);

