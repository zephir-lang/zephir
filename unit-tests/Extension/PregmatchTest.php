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

use Test\Pregmatch;

class PregmatchTest extends \PHPUnit_Framework_TestCase
{
    public function testPregMatch()
    {
        $t = new Pregmatch();
        $this->assertSame(1, $t->testWithoutReturnAndMatches());
        $this->assertSame(array('def'), $t->testWithoutReturns());
        $this->assertSame(1, $t->testWithoutMatches());
        $this->assertSame(1, $t->testPregMatchAll());
        $this->assertSame(1, $t->testPregMatchFallback());

        // more tests from php5 ext/pcre
        $string = 'Hello, world. [*], this is \ a string';
        $match1 = null;
        $this->assertSame(1, $t->testPregMatch3Params('/^[hH]ello,\s/', $string, $match1)); //finds "Hello, "
        $this->assertSame("Hello, ", $match1[0]);

        $match2 = null;
        $this->assertSame(0, $t->testPregMatch4Params('/l^o,\s\w{5}/', $string, $match2, PREG_OFFSET_CAPTURE)); // tries to find "lo, world" at start of string
        $this->assertCount(0, $match2);

        $match3 = null;
        $this->assertSame(1, $t->testPregMatch3Params('/\[\*\],\s(.*)/', $string, $match3)); //finds "[*], this is \ a string";
        $this->assertCount(2, $match3);

        $match4 = null;
        $this->assertSame(1, $t->testPregMatch5Params('@\w{4}\s\w{2}\s\\\(?:\s.*)@', $string, $match4, PREG_OFFSET_CAPTURE, 14)); //finds "this is \ a string" (with non-capturing parentheses)
        /**
         * @todo didn`t pass at local machine
         */
        $this->assertSame('this is \ a string', $match4[0][0]);

        $match5 = null;
        $this->assertSame(0, $t->testPregMatch3Params('/hello world/', $string, $match5)); //tries to find "hello world" (should be Hello, world)
        $this->assertCount(0, $match5);

        $string2 = "My\nName\nIs\nStrange";
        $match6 = null;
        $this->assertSame(1, $t->testPregMatch3Params("/M(.*)/", $string2, $match6));
        $this->assertCount(2, $match6);

        $this->assertSame(1, $t->testPregMatch2Params("#asd#", "asd"));
    }

    /**
     * @link https://github.com/phalcon/zephir/issues/287
     */
    public function testCollectMatches()
    {
        $t = new Pregmatch;
        $this->assertSame(array('asd'), $t->testPregMatchSaveMatches("asd", "#asd#"));
    }
    
    /**
     * @link https://github.com/phalcon/zephir/issues/144
     */
    public function testPregMatchAllFlags()
    {
        $t = new Pregmatch;
        $arr = $t->testMatchAllInZep();
        $this->assertEquals($arr[0], array(array('test1', 'test2'), array('test1', 'test2')));
        $this->assertEquals($arr[1], array(array('test1', 'test1'), array('test2', 'test2')));
    }
}
