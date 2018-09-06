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

use Test\Optimizers\CreateArray;

class CreateArrayTest extends \PHPUnit_Framework_TestCase
{
    public function testCreateArrayNoSize()
    {
        $t = new CreateArray;
        $this->assertSame(count($t->createNoSize()), 0);
    }

    public function testCreateArraySize()
    {
        $t = new CreateArray;
        $this->assertSame(count($t->createSize(3)), 3);
        $this->assertSame(count($t->createSize(10)), 10);
    }
}
