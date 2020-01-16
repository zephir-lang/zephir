<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Test\Optimizers\IsScalar;

class IsScalarTest extends TestCase
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

        $this->assertFalse($t->testVarParameter([]));
        $this->assertFalse($t->testVarParameter([1, 2, 3, 4, 5]));
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
