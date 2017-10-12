<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class AssignTest extends \PHPUnit_Framework_TestCase
{
    public function getComplexArrayTestValue1()
    {
        return array(
            'a' => array(
                'b_key' => 'b_val',
                'b' => array('d_key' => 'd_val', 'c' => array('d' => array('e' => 'f')))
            ),
            's' => 1,
            1 => array(2 => array(3 => 4, 5 => 6, 'abc' => 'abc'))
        );
    }

    public function getComplexArrayTestValue2()
    {
        return array(
            'a' => array(
                'b_key' => 'b_val',
                'b' => array('d_key' => 'd_val', 'c' => array('d' => array('e' => 'f')))
            ),
            1 => array(2 => array(3 => 4, 5 => 6, 'abc' => 'abc')),
            's' => 1
        );
    }

    public function testAssign()
    {
        $t = new \Test\Assign();

        $this->assertSame($t->testAssign1(), 1);
        $this->assertSame($t->testAssign2(), 1);
        $this->assertSame($t->testAssign3(), 0);
        $this->assertSame($t->testAssign4(), 0);
        $this->assertSame($t->testAssign5(), 2);
        $this->assertTrue($t->testAssign6());
        $this->assertTrue($t->testAssign7());
        $this->assertTrue($t->testAssign8());
        $this->assertFalse($t->testAssign9());
        $this->assertFalse($t->testAssign10());
        $this->assertSame($t->testAssign11(), 0.0);
        $this->assertSame($t->testAssign12(), 4.0);
        $this->assertSame($t->testAssign13(), 0.0);
        $this->assertSame($t->testAssign14(), 1.0);
        $this->assertSame($t->testAssign15(), 5.0);
        $this->assertSame($t->testAssign16(), 1);
        $this->assertSame($t->testAssign17(), 1.0);
        $this->assertFalse($t->testAssign18());
        $this->assertTrue($t->testAssign19());
        $this->assertNull($t->testAssign20());
        $this->assertSame($t->testAssign21(), 1);
        $this->assertSame($t->testAssign22(), 1.0);
        $this->assertTrue($t->testAssign23());
        $this->assertSame($t->testAssign24(), 1.0);
        $this->assertSame($t->testAssign25(), 1.0);
        $this->assertSame($t->testAssign26(), 1);
        $this->assertSame($t->testAssign27(), 1.0);
        $this->assertTrue($t->testAssign28());
        $this->assertTrue($t->testAssign29());
        $this->assertSame($t->testAssign30(), 0);
        $this->assertSame($t->testAssign31(), 0.0);
        $this->assertFalse($t->testAssign32());
        $this->assertFalse($t->testAssign33());
        $this->assertFalse($t->testAssign34());
        $this->assertFalse($t->testAssign35());
        $this->assertFalse($t->testAssign36());
        $this->assertSame($t->testAssign37(), $this->getComplexArrayTestValue1());
        $this->assertSame($t->testAssign38('i'), array('i' => 'val'));
    }

    public function testPropertyAssign()
    {
        $t = new \Test\Assign();

        $this->assertSame($t->testPropertyIncr1(), 2);
        $this->assertSame($t->testPropertyAddAssign1(), 2);
        $this->assertSame($t->testPropertyAddAssign2(), 3);
        $this->assertSame($t->testPropertyAssignValuePlus1(), 2);
        $this->assertSame($t->testPropertyDecr(), 1);
        $this->assertSame($t->testPropertySubAssign1(), -2);
        $this->assertSame($t->testPropertySubAssign2(), -1);
        $this->assertSame($t->testPropertyMulAssign1(), 2);
        $this->assertSame($t->testPropertyMulAssign2(), 3);
        $this->assertSame($t->testPropertyAssignStringConcat(), 'test string');
        $this->assertSame($t->testArrayVarAssign1('test_index', 'value'), array('test_index' => 'value'));
        $this->assertSame($t->testArrayVarAssign2('test_index', 'value'), array('test_index' => 'value'));
        $this->assertSame($t->testPropertyArray14(), $this->getComplexArrayTestValue2());
        $this->assertSame($t->testStaticPropertyArrayMulti4(), $this->getComplexArrayTestValue2());
        $this->assertSame($t->testStaticPropertyArrayAppend(), array("test", 1, 1.5, false, array()));
        $this->assertSame($t->testArrayBoolExpressionAssign(), array("a" => true, "b" => false));
    }

    public function testGlobalVarAssign()
    {
        $t = new \Test\Assign();

        /**
         * @see LetStatement::_assignArrayIndexSingle();
         */
        $this->assertTrue(!isset($_POST['test_index']));
        $t->testGlobalVarAssign('test_index', 'value');
        $this->assertSame($_POST['test_index'], 'value');

        /* Check primitive types */
        $t->testAssignSuperGlobals();
        $this->assertSame($_GET['steststr'], 'stest');
        $this->assertSame($_GET['steststr2'], 'stest2');
        $this->assertSame($_GET['stestint'], 1);
        $this->assertSame($_GET['stestint2'], 2);
        $this->assertSame($_GET['stest2'], 'testval');
    }
}
