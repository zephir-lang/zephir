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

class JsonTest extends TestCase
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
