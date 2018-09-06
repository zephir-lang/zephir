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

namespace Extension\Optimizers;

use Test\Optimizers\ArrayMerge;

class ArrayMergeTest extends \PHPUnit_Framework_TestCase
{
    public function testTwoArrays()
    {
        $this->assertSame(array(1, 2, 3, 4, 5), ArrayMerge::mergeTwoRequiredArrays(array(1, 2, 3), array(4, 5)));
        $this->assertSame(array(1, 2, 3), ArrayMerge::mergeTwoRequiredArrays(array(1, 2, 3), array()));
        $this->assertSame(array(1, 2, 3), ArrayMerge::mergeTwoRequiredArrays(array(), array(1, 2, 3)));
    }
}
