<?php

$t = new Test\ScallExternal();

assert($t->testCall1() == "hello public");
assert($t->testCall4(2, 3) === 5);
