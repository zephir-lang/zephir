<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Pregmatch;

class PregmatchTest extends TestCase
{
    /** @var Pregmatch */
    private $test;

    public function setUp()
    {
        $this->test = new Pregmatch();
    }

    public function testPregMatch()
    {
        $this->assertSame(1, $this->test->testWithoutReturnAndMatches());
        $this->assertSame(['def'], $this->test->testWithoutReturns());
        $this->assertSame(1, $this->test->testWithoutMatches());
        $this->assertSame(1, $this->test->testPregMatchAll());
        $this->assertSame(1, $this->test->testPregMatchFallback());

        // more tests from php5 ext/pcre

        // TODO: Implement References (reference for parameters in this case)
        // without references it's not possible to get the "matches" value out of the scope
        // of the test function, since it's passed by value to testPregMatch3Params for example
        // and any value remains only in the scope of the test function

        if (version_compare(PHP_VERSION, '7.0', '<')) {
            $string = 'Hello, world. [*], this is \ a string';
            $match1 = null;
            $this->assertSame(1, $this->test->testPregMatch3Params('/^[hH]ello,\s/', $string, $match1)); //finds "Hello, "
            $this->assertSame('Hello, ', $match1[0]);

            $match2 = null;
            $this->assertSame(0, $this->test->testPregMatch4Params('/l^o,\s\w{5}/', $string, $match2, PREG_OFFSET_CAPTURE)); // tries to find "lo, world" at start of string
            $this->assertCount(0, $match2);

            $match3 = null;
            $this->assertSame(1, $this->test->testPregMatch3Params('/\[\*\],\s(.*)/', $string, $match3)); //finds "[*], this is \ a string";
            $this->assertCount(2, $match3);

            $match4 = null;
            $this->assertSame(1, $this->test->testPregMatch5Params('@\w{4}\s\w{2}\s\\\(?:\s.*)@', $string, $match4, PREG_OFFSET_CAPTURE, 14)); //finds "this is \ a string" (with non-capturing parentheses)
            /*
             * @todo didn`this->test pass at local machine
             */
            $this->assertSame('this is \ a string', $match4[0][0]);

            $match5 = null;
            $this->assertSame(0, $this->test->testPregMatch3Params('/hello world/', $string, $match5)); //tries to find "hello world" (should be Hello, world)
            $this->assertCount(0, $match5);

            $string2 = "My\nName\nIs\nStrange";
            $match6 = null;
            $this->assertSame(1, $this->test->testPregMatch3Params('/M(.*)/', $string2, $match6));
            $this->assertCount(2, $match6);
        }

        $this->assertSame(1, $this->test->testPregMatch2Params('#asd#', 'asd'));
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/287
     */
    public function testCollectMatches()
    {
        $this->assertSame(['asd'], $this->test->testPregMatchSaveMatches('asd', '#asd#'));
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/144
     */
    public function testPregMatchAllFlags()
    {
        $arr = $this->test->testMatchAllInZep();
        $this->assertSame($arr[0], [['test1', 'test2'], ['test1', 'test2']]);
        $this->assertSame($arr[1], [['test1', 'test1'], ['test2', 'test2']]);
    }
}
