<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
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
