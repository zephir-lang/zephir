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

class JsonTest extends \PHPUnit_Framework_TestCase
{
    public function testEncode()
    {
        $t = new \Test\Json();

        $this->assertTrue($t->testEncodeObject() == '{"a":"hello","b":"world","c":128}');
        $this->assertTrue($t->testEncodeArray() == '[1,2,3]');
        $this->assertTrue($t->testEncodeOptions() == '["\\u003Cfoo\\u003E","\'bar\'","&blong&","\\u00e9"]');
    }

    public function testDecode()
    {
        $t = new \Test\Json();

        $obj1 = $t->testDecodeObject();
        $this->assertTrue($obj1->a == 'hello');

        $arr1 = $t->testDecodeArray();
        $this->assertTrue($arr1[1] == 2);

        $obj2 = $t->testDecodeObject2();
        $this->assertTrue($obj2['a'] == 'hello');
    }
}
