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

class OperatorTest extends \PHPUnit_Framework_TestCase
{
    public function test404Issue()
    {
        $t = new \Test\Operator();

        $this->assertFalse($t->testIdentical(true, 1));
        $this->assertFalse($t->testIdentical(true, 'phalcon'));
        $this->assertFalse($t->testIdentical(false, 0));
        $this->assertFalse($t->testIdentical(1, '1'));
        $this->assertTrue($t->testIdentical(1, 1));
        $this->assertTrue($t->testIdentical(true, true));
        $this->assertTrue($t->testIdentical('phalcon', 'phalcon'));
    }

    public function test829Issue()
    {
        $t = new \Test\Operator();

        $this->assertTrue($t->testIdenticalVarFalse(false));
        $this->assertFalse($t->testIdenticalVarFalse(0));
        $this->assertFalse($t->testIdenticalVarFalse(''));
        $this->assertTrue($t->testIdenticalFalseVar(false));
        $this->assertFalse($t->testIdenticalFalseVar(0));
        $this->assertFalse($t->testIdenticalFalseVar(''));
    }

    public function test1072Issue()
    {
        $t = new \Test\Operator();

        $this->assertFalse($t->testIdenticalIfComplex(false));
        $this->assertTrue($t->testIdenticalIfComplex(0));
        $this->assertTrue($t->testIdenticalIfComplex(''));
    }

    public function test1279Issue()
    {
        $t = new \Test\Operator();

        $values = $t->testBoolComparison(true, false);

        $this->assertTrue($values[0]);
        $this->assertTrue($values[1]);
        $this->assertTrue($values[2]);
        $this->assertTrue($values[3]);
        $this->assertTrue($values[4]);
        $this->assertTrue($values[5]);
        $this->assertTrue($values[6]);
        $this->assertTrue($values[7]);
    }
}
