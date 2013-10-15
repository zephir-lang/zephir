<?php

$t = new Test\NativeArray();

assert($t->testArray1() == array());
assert($t->testArray2() == array(1, 2, 3));
assert($t->testArray3() == array(1.1, 2.2, 3.3));
assert($t->testArray4() == array(false, true, false));
assert($t->testArray5() == array(null, null, null));
assert($t->testArray6() == array("x", "y", "z"));
assert($t->testArray7() == array(1, 2, 3));
assert($t->testArray8() == array(1.0, 2.0, 3.0));
assert($t->testArray9() == array(true, false, true));
assert($t->testArray10() == array("hello1",  "hello2",  "hello3"));
assert($t->testArray11() == array(array(1, 2, 3), array(4, 5, 6)));
assert($t->testArray12() == array(array(array(1, 2, 3))));
assert($t->testArray13() == array(1 => "hello1", 2 => "hello2", 3 => "hello3"));
assert($t->testArray14() == array("hello1" => 1, "hello2" => 2, "hello3" => 3));
assert($t->testArray15() == array("hello1" => true, "hello2" => false, "hello3" => true));
assert($t->testArray16() == array("hello1" => 1.0, "hello2" => 2.0, "hello3" => 3.0));
assert($t->testArray17() == array("hello1" => null, "hello2" => null, "hello3" => null));
assert($t->testArray18() == array("hello1" => "a", "hello2" => "b", "hello3" => "c"));
assert($t->testArray19() == array(0 => true, 1 => false, 2 => true));
assert($t->testArray20() == array(0 => 1.0, 1 => 2.0, 2 => 3.0));
assert($t->testArray21() == array(0 => null, 1 => null, 2 => null));
assert($t->testArray22() == array(0 => 4, 1 => 5, 2 => 6));
assert($t->testArray23() == array(0 => 0, 1 => 0, 2 => 0));
assert($t->testArray24() == array(0 => 0.0, 1 => 0.0, 2 => 0.0));

assert($t->testArrayAccess1() == 1);
assert($t->testArrayAccess2() == 1);
assert($t->testArrayAccess3() == 1);
assert($t->testArrayAccess4() == 1);
assert($t->testArrayAccess5() == 1);

assert($t->testArrayMultipleAccess1() == "a");
assert($t->testArrayMultipleAccess2() == "b");
assert($t->testArrayMultipleAccess3() == "b");
assert($t->testArrayMultipleAccess4() == "b");
assert($t->testArrayMultipleAccess5() == 0);

assert($t->testArrayUpdate1() == array(4, 2, 3));
assert($t->testArrayUpdate2() == array(4, 2, 3));
assert($t->testArrayUpdate3() == array(4, 5, 3));
assert($t->testArrayUpdate4() == array("a" => 4, "b" => 2, "c" => 3));

assert($t->testMultipleArrayUpdate1() == array('x' => array('y' => NULL)));
assert($t->testMultipleArrayUpdate2() == array('x' => array('y' => array('z' => NULL))));
assert($t->testMultipleArrayUpdate3() == array(0 => array(1 => NULL)));
assert($t->testMultipleArrayUpdate4() == array(0 => array(1 => array(2 => NULL))));
assert($t->testMultipleArrayUpdate5() == array('a' => array(1 => array('c' => NULL))));
assert($t->testMultipleArrayUpdate6() == array(0 => array('b' => array(2 => NULL))));
assert($t->testMultipleArrayUpdate7() == array('a' => array(1 => array('c' => true)), 0 => array('b' => array(2 => false))));
assert($t->testMultipleArrayUpdate8() == array('a' => array(0 => NULL, 1 => true, 2 => false)));
assert($t->testMultipleArrayUpdate9() == array('a' => array(0 => NULL, 1 => false)));
assert($t->testMultipleArrayUpdate10() == array('a' => array(0 => NULL, 1 => array('c' => false))));
assert($t->testMultipleArrayUpdate11() == array('y' => array('x' => NULL)));
assert($t->testMultipleArrayUpdate12() == array('y' => array('x' => array('z' => NULL))));
