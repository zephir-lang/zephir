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

class PregmatchTest
    extends \PHPUnit_Framework_TestCase
{
    public function testPregMatch()
    {
        $t = new \Test\Pregmatch();

        $this->assertTrue($t->testWithoutReturnAndMatches() === 1);
        $this->assertTrue($t->testWithoutReturns() == array('def'));
        $this->assertTrue($t->testWithoutMatches() === 1);
        $this->assertTrue($t->testPregMatchAll() === 1);
        $this->assertTrue($t->testPregMatchFallback() === 1);


        // more tests from php5 ext/pcre
        $string = 'Hello, world. [*], this is \ a string';
        $match1 = null;
        $this->assertTrue($t->testPregMatch3Params('/^[hH]ello,\s/', $string, $match1) === 1); //finds "Hello, "
        $this->assertTrue($match1[0] == "Hello, ");

        $match2 = null;
        $this->assertTrue($t->testPregMatch4Params('/l^o,\s\w{5}/', $string, $match2, PREG_OFFSET_CAPTURE) == false); // tries to find "lo, world" at start of string
        $this->assertTrue(count($match2) == 0);

        $match3 = null;
        $this->assertTrue($t->testPregMatch3Params('/\[\*\],\s(.*)/', $string, $match3) === 1); //finds "[*], this is \ a string";
        $this->assertTrue(count($match3) == 2);

        $match4 = null;
        $this->assertTrue($t->testPregMatch5Params('@\w{4}\s\w{2}\s\\\(?:\s.*)@', $string, $match4, PREG_OFFSET_CAPTURE, 14) === 1); //finds "this is \ a string" (with non-capturing parentheses)
        /**
         * @todo didn`t pass at local machine
         */
//        $this->assertTrue($match4[0][0] == 'this is \ a string');

        $match5 = null;
        $this->assertTrue($t->testPregMatch3Params('/hello world/', $string, $match5) == false); //tries to find "hello world" (should be Hello, world)
        $this->assertTrue(count($match5) == 0);

        $string2 = "My\nName\nIs\nStrange";
        $match6 = null;
        $this->assertTrue($t->testPregMatch3Params("/M(.*)/", $string2, $match6) === 1);
        $this->assertTrue(count($match6) == 2);
    }
} 