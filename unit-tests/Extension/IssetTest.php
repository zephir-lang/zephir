<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class IssetTest extends TestCase
{
    public $b = 'a';
    private $test2 = 'b';

    public function testIssetArray()
    {
        $t = new \Test\IssetTest();
        $testArray = ['a', 'abc' => 'def', 'gef' => '123'];
        $this->assertTrue($t->testIssetArray1($testArray, 'abc'));
        $this->assertTrue(!$t->testIssetArray2($testArray, 12));
        $this->assertTrue($t->testIssetArray3($testArray, 'gef'));
        $this->assertTrue($t->testIssetArray4($testArray));
        $this->assertTrue(!$t->testIssetArray5($testArray));
    }

    public function testIssetProperties()
    {
        $t = new \Test\IssetTest();
        $this->assertTrue($t->testIssetProperty1($this));
        $this->assertTrue($t->testIssetProperty2($this, 'test2'));
        $this->assertTrue(!$t->testIssetProperty2($this, 'test3'));
        $this->assertTrue($t->testIssetProperty3($this));
    }

    public function testIssetDynamicProperty()
    {
        $t = new \Test\IssetTest();
        $this->assertTrue($t->testIssetDynamicProperty1());
        $this->assertTrue(!$t->testIssetDynamicProperty2($this));
        $this->s = ['a' => 'true'];
        $this->assertTrue($t->testIssetDynamicProperty2($this));
    }
}
