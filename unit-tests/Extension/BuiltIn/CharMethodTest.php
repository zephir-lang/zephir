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

namespace Extension\BuiltIn;

use Test\BuiltIn\CharMethods;

class CharMethodTest extends \PHPUnit_Framework_TestCase
{
    public function testModifications()
    {
        $t = new CharMethods();

        $this->assertSame('61', $t->getHex());
        $this->assertSame('68656C6C6F', $t->getHexForString('hello'));
    }
}
