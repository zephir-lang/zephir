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

class ExitDieTest extends \PHPUnit_Framework_TestCase
{
    public function testExitDie()
    {
        $testfile1 = __DIR__ .'/fixtures/exit.php';
        $return1 = `php $testfile1`;
        $this->assertEquals("", trim($return1));


        $arg = "Hello World";
        $testfile2 = __DIR__ .'/fixtures/exit_string.php';
        $return2 = `php $testfile2 "$arg"`;
        $this->assertEquals($arg, trim($return2));


        $testfile3 = __DIR__ .'/fixtures/exit_int.php';
        $int_arg = 128;
        $return3 = `php $testfile3 $int_arg || echo "$?"`;
        $return3 = intval(trim($return3));
        $this->assertTrue($return3 == $int_arg);
    }
}
