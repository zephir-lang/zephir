<?php

$t = new Test\BuiltInMethods();

assert($t->stringMethodLength1() == 5);
assert($t->stringMethodLength2() == 5);
assert($t->stringMethodLength3() == 10);
assert($t->stringMethodLength4("hello") == 5);
assert($t->stringMethodLength5("hello") == 10);

assert($t->stringMethodTrim1() == "hello");

assert($t->intMethodAbs1() == 100);
assert($t->intMethodAbs2() == 50);