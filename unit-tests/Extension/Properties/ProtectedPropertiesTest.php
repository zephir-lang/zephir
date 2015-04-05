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

namespace Extension\Properties;

class ProtectedPropertiesTEst extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Properties\ProtectedProperties();

        $this->assertTrue($t->getSomeNull() === null);
        $this->assertTrue($t->getSomeNullInitial() === null);
        $this->assertTrue($t->getSomeFalse() === false);
        $this->assertTrue($t->getSomeTrue() === true);
        $this->assertTrue($t->getSomeInteger() === 10);
        $this->assertTrue($t->getSomeDouble() === 10.25);
        $this->assertTrue($t->getSomeString() === "test");

        $t->setSomeVar(($rand = rand(1, 1000)*100));
        $this->assertTrue($t->getSomeVar() === $rand);
    }
}
