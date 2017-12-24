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

namespace Extension\Integral;

class SortTest extends \PHPUnit_Framework_TestCase
{
    public function testSort()
    {
        $data = $original = range(1, 50);
        shuffle($data);

        $t = new \Test\Sort();
        $this->assertSame($t->quick($data), $original);
    }
}
