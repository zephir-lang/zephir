<?php

$t = new Test\Assign();

assert($t->testAssign1() === 1);
assert($t->testAssign2() === 1);
assert($t->testAssign3() === 0);
assert($t->testAssign4() === 0);
assert($t->testAssign5() === 2);
assert($t->testAssign6() === true);
assert($t->testAssign7() === true);
assert($t->testAssign8() === true);
assert($t->testAssign9() === false);
assert($t->testAssign10() === false);
assert($t->testAssign11() === 0.0);
assert($t->testAssign12() === 4.0);
assert($t->testAssign13() === 0.0);
assert($t->testAssign14() === 1.0);
assert($t->testAssign15() === 5.0);
assert($t->testAssign16() === 1);
assert($t->testAssign17() === 1.0);
assert($t->testAssign18() === false);
assert($t->testAssign19() === true);
assert($t->testAssign20() === null);
assert($t->testAssign21() === 1);
assert($t->testAssign22() === 1.0);
assert($t->testAssign23() === true);
assert($t->testAssign24() === 1.0);
