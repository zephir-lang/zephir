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

namespace Extension\Optimizers;

use Test\Optimizers\Math;

class MathTest extends \PHPUnit_Framework_TestCase
{
    public function testSqrt()
    {
        $t = new Math();

        $this->assertSame(2.0, $t->testSqrtInt());
        $this->assertSame(2.0, $t->testSqrtVar());

        $this->assertSame(2.0, $t->testSqrtIntValue1());
        $this->assertSame(4.0, $t->testSqrtIntValue2());

        $this->assertSame(2.0, $t->testSqrtIntParameter(4));
        $this->assertSame(4.0, $t->testSqrtIntParameter(16));

        $this->assertSame(2.0, $t->testSqrtVarParameter(4));
        $this->assertSame(4.0, $t->testSqrtVarParameter(16));
    }
}
