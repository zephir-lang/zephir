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

namespace Extension\Optimizers;

use Test\Optimizers\Substr;

class SubstrTest extends \PHPUnit_Framework_TestCase
{
    public function testTwoArguments1()
    {
        $t = new Substr();

        $strings_array = array(null, '', 12345, 'abcdef', '123abc', '_123abc');
        $results1 = array(false, false, '2345', 'bcdef', '23abc', '123abc');
        $results2 = array(false, false, '12345', 'abcdef', '123abc', '_123abc');
        $results3 = array(false, false, '45', 'ef', 'bc', 'bc');

        $c = 0;
        foreach ($strings_array as $str) {
            $this->assertSame($t->testTwoArguments($str, 1), $results1[$c]);
            $this->assertSame($t->testTwoArguments($str, 0), $results2[$c]);
            $this->assertSame($t->testTwoArguments($str, -2), $results3[$c]);
            ++$c;
        }
    }

    public function testThreeArguments1()
    {
        $t = new Substr();
        $this->assertSame($t->testThreeArguments('abcdef', 0, -3), 'abc');
        $this->assertSame($t->testThreeArguments('123abc', 0, 3), '123');
        $this->assertSame($t->testThreeArguments('abcdef', 1, -3), 'bc');
        $this->assertSame($t->testThreeArguments('abcdef', -2, 0), '');
        $this->assertSame($t->testThreeArguments(12345, 1, -1), '234');
    }
}
