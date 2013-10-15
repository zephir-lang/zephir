<?php

$t = new Test\McallChained();

assert($t->testChained1() === $t);
assert($t->testChained2() === $t);
assert($t->testChained3() === $t);

