<?php

$t = new Test\BuiltInMethods();

assert($t->stringMethodLength1(), 5);
assert($t->stringMethodLength2(), 5);
assert($t->stringMethodLength3(), 10);
assert($t->stringMethodLength4("hello"), 5);
assert($t->stringMethodLength4("hello"), 10);