<?php

$t = new Test\Flow();

assert($t->testIf1() === 1);
assert($t->testIf2() === 0);
assert($t->testIf3() === 1);
assert($t->testIf4() === 0);
assert($t->testIf5() === 1);
assert($t->testIf6() === 0);
assert($t->testIf7() === 1);
assert($t->testIf8() === 0);
assert($t->testIf9() === 1);
assert($t->testIf10() === 1);

assert($t->testLoop1() === true);
assert($t->testLoop2() === 5);
assert($t->testLoop3() === 5);

assert($t->testWhile1() === 0);
assert($t->testWhile2() === 0);
assert($t->testWhile3() == 0.0);
assert($t->testWhile4() == 0.0);
assert($t->testWhile5() == 0.0);
assert($t->testWhile6() == 0.0);
assert($t->testWhile7() == 0.0);
assert($t->testWhile8() == 0.0);
assert($t->testWhile9() == 0.0);
