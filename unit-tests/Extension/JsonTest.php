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

namespace Extension;

class JsonTest extends \PHPUnit_Framework_TestCase
{
    public function testEncode()
    {
        $t = new \Test\Json();

        $this->assertSame($t->testEncodeObject(), '{"a":"hello","b":"world","c":128}');
        $this->assertSame($t->testEncodeArray(), '[1,2,3]');
        $this->assertSame($t->testEncodeOptions(), '["\\u003Cfoo\\u003E","\'bar\'","&blong&","\\u00e9"]');
    }

    public function testDecode()
    {
        $t = new \Test\Json();

        $obj1 = $t->testDecodeObject();
        $this->assertSame($obj1->a, 'hello');

        $arr1 = $t->testDecodeArray();
        $this->assertSame($arr1[1], 2);

        $obj2 = $t->testDecodeObject2();
        $this->assertSame($obj2['a'], 'hello');
    }
}
