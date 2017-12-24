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

namespace Extension\Properties;

class ProtectedPropertiesTEst extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Properties\ProtectedProperties();

        $this->assertNull($t->getSomeNull());
        $this->assertNull($t->getSomeNullInitial());
        $this->assertFalse($t->getSomeFalse());
        $this->assertTrue($t->getSomeTrue());
        $this->assertSame($t->getSomeInteger(), 10);
        $this->assertSame($t->getSomeDouble(), 10.25);
        $this->assertSame($t->getSomeString(), 'test');

        $t->setSomeVar(($rand = rand(1, 1000)*100));
        $this->assertSame($t->getSomeVar(), $rand);
    }
}
