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

class IssetTest extends \PHPUnit_Framework_TestCase
{
    public $b = "a";
    private $test2 = "b";

    public function testIssetArray()
    {
        $t = new \Test\IssetTest();
        $testArray = array("a", "abc" => "def", "gef" => "123");
        $this->assertTrue($t->testIssetArray1($testArray, "abc"));
        $this->assertTrue(!$t->testIssetArray2($testArray, 12));
        $this->assertTrue($t->testIssetArray3($testArray, "gef"));
        $this->assertTrue($t->testIssetArray4($testArray));
        $this->assertTrue(!$t->testIssetArray5($testArray));
    }

    public function testIssetProperties()
    {
        $t = new \Test\IssetTest();
        $this->assertTrue($t->testIssetProperty1($this));
        $this->assertTrue($t->testIssetProperty2($this, "test2"));
        $this->assertTrue(!$t->testIssetProperty2($this, "test3"));
        $this->assertTrue($t->testIssetProperty3($this));
    }

    public function testIssetDynamicProperty()
    {
        $t = new \Test\IssetTest();
        $this->assertTrue($t->testIssetDynamicProperty1());
        $this->assertTrue(!$t->testIssetDynamicProperty2($this));
        $this->s = array("a" => "true");
        $this->assertTrue($t->testIssetDynamicProperty2($this));
    }
}
