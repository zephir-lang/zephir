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

assert($t->testFor1() === 10);
assert($t->testFor2() === 6.0);
assert($t->testFor3() === array(4, 3, 2, 1));
assert($t->testFor4() === 55);
assert($t->testFor5() === 55);
assert($t->testFor6() === 55);
assert($t->testFor7() === 55);
assert($t->testFor8() === 55);
assert($t->testFor9() === 55);
assert($t->testFor10() === 55);
assert($t->testFor11() === 'abcdefghijklmnopqrstuvwxyz');
assert($t->testFor12() === 'zyxwvutsrqponmlkjihgfedcba');
assert($t->testFor13() === '0123456789');
assert($t->testFor14() === '9876543210');
assert($t->testFor15(1, 10) === 55);

assert($t->testFor16() === array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10));
assert($t->testFor17() === array(0 => 10, 1 => 9, 2 => 8, 3 => 7, 4 => 6, 5 => 5, 6 => 4, 7 => 3, 8 => 2, 9 => 1));
assert($t->testFor18() === array(0 => 1, 1 => 2, 2 => 3, 3 => 4, 4 => 5, 5 => 6, 6 => 7, 7 => 8, 8 => 9, 9 => 10));

assert($t->testFor19() === 25);
assert($t->testFor20() === 25);
assert($t->testFor21() === 0);
assert($t->testFor22() === 0);
assert($t->testFor23() === "zxvtrpnljhfdb");

assert($t->testSwitch1() === 10);
assert($t->testSwitch2() === true);
assert($t->testSwitch3() === false);
assert($t->testSwitch4() === 2);
assert($t->testSwitch5() === 1);
assert($t->testSwitch6() === 1);
assert($t->testSwitch7() === 1);
assert($t->testSwitch8() === 2);
assert($t->testSwitch9() === 2);
