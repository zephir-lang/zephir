<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

class ReferencesTest extends \PHPUnit_Framework_TestCase
{
    public function testScalarChange()
    {
        $test = new \Test\References();
        $num = 5;
        $str = 'foo';
        $bool = true;

        $this->assertEqual($test->intChange($num, 10), $num);
        $this->assertEqual($test->stringChange($str, 'bar'), $str);
        $this->assertEqual($test->boolChange($bool, false), $bool);
    }

    public function testArrayChange()
    {
        $test = new \Test\References();
        $arr = array('foo', 'bar');

        $this->assertEqual($test->arrayChange($arr, 0, 'bar'), $arr[0]);
        $this->assertEqual($test->arrayChange($arr, 1, 'foo'), $arr[1]);
    }
}
