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

class CharsTest extends \PHPUnit_Framework_TestCase
{
    public function testSumChars()
    {
        $t = new \Test\Chars();

        $this->assertSame(chr($t->sumChars1()), 'a');
        $this->assertSame(chr($t->sumChars2('A')), 'a');
    }

    public function testDiffChars()
    {
        $t = new \Test\Chars();

        $this->assertSame(chr($t->diffChars1()), 'A');
        $this->assertSame(chr($t->diffChars2('a')), 'A');
    }
}
