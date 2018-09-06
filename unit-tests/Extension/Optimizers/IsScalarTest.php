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

namespace Extension\Optimizers;

use Test\Optimizers\IsScalar;

class IsScalarTest extends \PHPUnit_Framework_TestCase
{
    public function testVariable()
    {
        $t = new IsScalar();

        $this->assertTrue($t->testIntVar());
        $this->assertTrue($t->testDoubleVar());
        $this->assertTrue($t->testBoolVar());
        $this->assertTrue($t->testStringVar());
        $this->assertTrue($t->testVar());

        $this->assertFalse($t->testEmptyArrayVar());
    }

    public function testVariableParameter()
    {
        $t = new IsScalar();

        $this->assertTrue($t->testVarParameter(1));
        $this->assertTrue($t->testVarParameter(1.0));
        $this->assertTrue($t->testVarParameter(true));
        $this->assertTrue($t->testVarParameter(false));
        $this->assertTrue($t->testVarParameter(''));
        $this->assertTrue($t->testVarParameter('test string'));

        $this->assertFalse($t->testVarParameter(array()));
        $this->assertFalse($t->testVarParameter(array(1,2,3,4,5)));
        $this->assertFalse($t->testVarParameter($this));
        $this->assertFalse(
            $t->testVarParameter(
                function () {
                }
            )
        );
        $this->assertFalse($t->testVarParameter(null));
    }
}
