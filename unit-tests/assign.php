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
assert($t->testAssign25() === 1.0);
assert($t->testAssign26() === 1);
assert($t->testAssign27() === 1.0);
assert($t->testAssign28() === true);
assert($t->testAssign29() === true);
assert($t->testAssign30() === 0);
assert($t->testAssign31() === 0.0);
assert($t->testAssign32() === false);
assert($t->testAssign33() === false);
assert($t->testAssign34() === false);
assert($t->testAssign35() === false);
assert($t->testAssign36() === false);

assert($t->testPropertyIncr1() === 2);
assert($t->testPropertyAddAssign1() === 2);
assert($t->testPropertyAddAssign2() === 3);
assert($t->testPropertyAssignValuePlus1() === 2);

assert($t->testPropertyDecr() === 1);
assert($t->testPropertySubAssign1() === -2);
assert($t->testPropertySubAssign2() === -1);

assert($t->testPropertyMulAssign1() === 2);
assert($t->testPropertyMulAssign2() === 3);

assert($t->testPropertyAssignStringConcat() === 'test string');
assert($t->testArrayVarAssign1('test_index', 'value') == array('test_index' => 'value'));
assert($t->testArrayVarAssign2('test_index', 'value') == array('test_index' => 'value'));

/**
 * @see LetStatement::_assignArrayIndexSingle();
 */
assert(!isset($_POST['test_index']));
$t->testGlobalVarAssign('test_index', 'value');
assert($_POST['test_index'] == 'value');