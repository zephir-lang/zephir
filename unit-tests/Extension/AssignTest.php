<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class AssignTest extends \PHPUnit_Framework_TestCase
{
    public function getComplexArrayTestValue()
    {
        return array(
            1 => array(2 => array(3 => 4, 5 => 6, "abc" => "abc")),
            "a" => array(
                "b_key" => "b_val",
                "b" => array("d_key" => "d_val", "c" => array("d" => array("e" => "f")))
            ),
            "s" => 1
        );
    }

    public function testAssign()
    {
        $t = new \Test\Assign();

        $this->assertTrue($t->testAssign1() === 1);
        $this->assertTrue($t->testAssign2() === 1);
        $this->assertTrue($t->testAssign3() === 0);
        $this->assertTrue($t->testAssign4() === 0);
        $this->assertTrue($t->testAssign5() === 2);
        $this->assertTrue($t->testAssign6() === true);
        $this->assertTrue($t->testAssign7() === true);
        $this->assertTrue($t->testAssign8() === true);
        $this->assertTrue($t->testAssign9() === false);
        $this->assertTrue($t->testAssign10() === false);
        $this->assertTrue($t->testAssign11() === 0.0);
        $this->assertTrue($t->testAssign12() === 4.0);
        $this->assertTrue($t->testAssign13() === 0.0);
        $this->assertTrue($t->testAssign14() === 1.0);
        $this->assertTrue($t->testAssign15() === 5.0);
        $this->assertTrue($t->testAssign16() === 1);
        $this->assertTrue($t->testAssign17() === 1.0);
        $this->assertTrue($t->testAssign18() === false);
        $this->assertTrue($t->testAssign19() === true);
        $this->assertTrue($t->testAssign20() === null);
        $this->assertTrue($t->testAssign21() === 1);
        $this->assertTrue($t->testAssign22() === 1.0);
        $this->assertTrue($t->testAssign23() === true);
        $this->assertTrue($t->testAssign24() === 1.0);
        $this->assertTrue($t->testAssign25() === 1.0);
        $this->assertTrue($t->testAssign26() === 1);
        $this->assertTrue($t->testAssign27() === 1.0);
        $this->assertTrue($t->testAssign28() === true);
        $this->assertTrue($t->testAssign29() === true);
        $this->assertTrue($t->testAssign30() === 0);
        $this->assertTrue($t->testAssign31() === 0.0);
        $this->assertTrue($t->testAssign32() === false);
        $this->assertTrue($t->testAssign33() === false);
        $this->assertTrue($t->testAssign34() === false);
        $this->assertTrue($t->testAssign35() === false);
        $this->assertTrue($t->testAssign36() === false);
        $this->assertTrue($t->testAssign37() == $this->getComplexArrayTestValue());
        $this->assertTrue($t->testAssign38('i') == array('i' => 'val'));
    }

    public function testPropertyAssign()
    {
        $t = new \Test\Assign();

        $this->assertTrue($t->testPropertyIncr1() === 2);
        $this->assertTrue($t->testPropertyAddAssign1() === 2);
        $this->assertTrue($t->testPropertyAddAssign2() === 3);
        $this->assertTrue($t->testPropertyAssignValuePlus1() === 2);
        $this->assertTrue($t->testPropertyDecr() === 1);
        $this->assertTrue($t->testPropertySubAssign1() === -2);
        $this->assertTrue($t->testPropertySubAssign2() === -1);
        $this->assertTrue($t->testPropertyMulAssign1() === 2);
        $this->assertTrue($t->testPropertyMulAssign2() === 3);
        $this->assertTrue($t->testPropertyAssignStringConcat() === 'test string');
        $this->assertTrue($t->testArrayVarAssign1('test_index', 'value') == array('test_index' => 'value'));
        $this->assertTrue($t->testArrayVarAssign2('test_index', 'value') == array('test_index' => 'value'));
        $this->assertTrue($t->testPropertyArray14() == $this->getComplexArrayTestValue());
        $this->assertTrue($t->testStaticPropertyArrayMulti4() == $this->getComplexArrayTestValue());
        $this->assertTrue($t->testStaticPropertyArrayAppend() == array("test", 1, 1.5, false, array()));
        $this->assertTrue($t->testArrayBoolExpressionAssign() == array("a" => true, "b" => false));
    }

    public function testGlobalVarAssign()
    {
        $t = new \Test\Assign();

        /**
         * @see LetStatement::_assignArrayIndexSingle();
         */
        assert(!isset($_POST['test_index']));
        $t->testGlobalVarAssign('test_index', 'value');
        assert($_POST['test_index'] == 'value');

        /* Check primitive types */
        $t->testAssignSuperGlobals();
        assert($_GET["steststr"] == "stest");
        assert($_GET["steststr2"] == "stest2");
        assert($_GET["stestint"] == 1);
        assert($_GET["stestint2"] == 2);
        assert($_GET["stest2"] == "testval");
    }

    public function testAssignBitwiseX()
    {
        $a = 900; // 1110000100
        $b = 5;   // 0000000101

        $test = new \Test\Assign();

        foreach (array("or", "and", "xor", "shiftleft", "shiftright") as $op) {
            $this->assertEquals($test->testAssignBitwiseX($op, $a, $b), $this->resultNativeBitwiseX($op, $a, $b));
        }
    }

    protected function resultNativeBitwiseX($op, $a, $b)
    {
        $result = array();
        $r      = 0;

        switch ($op) {
            case 'or':
                $a |= $b;
                break;
            case 'and':
                $a &= $b;
                break;
            case 'xor':
                $a ^= $b;
                break;
            case 'shiftleft':
                $a <<= $b;
                break;
            case 'shiftright':
                $a >>= $b;
                break;
        }

        for ($i = 0; $i < 2; $i++) {
            for ($j = 0; $j < 2; $j++) {
                $r = $i;
                switch ($op) {
                    case 'or':
                        $r |= $j;
                        break;
                    case 'and':
                        $r &= $j;
                        break;
                    case 'xor':
                        $r ^= $j;
                        break;
                }

                $result[] = $r;
            }
        }

        $a += array_sum($result);
        return $a;
    }
}
