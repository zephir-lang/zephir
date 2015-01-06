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

namespace Extension\BuiltIn;

use Test\BuiltIn\CharMethods;

class CharMethodTest extends \PHPUnit_Framework_TestCase
{
    public function testModifications()
    {
        $t = new CharMethods();

        $this->assertEquals('61', $t->getHex());
        $this->assertEquals('68656C6C6F', $t->getHexForString('hello'));
    }
}
