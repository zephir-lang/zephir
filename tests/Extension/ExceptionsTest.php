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
use Stub\Exception;
use Stub\Exceptions;

class ExceptionsTest extends TestCase
{
    /** @var Exceptions */
    private $test;

    public function setUp()
    {
        $this->test = new Exceptions();
    }

    /**
     * @expectedException \Stub\Exception
     * @expectedExceptionMessage hello1
     */
    public function testException1()
    {
        $this->test->testException1();
    }

    /**
     * @expectedException \Stub\Exception
     * @expectedExceptionMessage hello2
     */
    public function testException2()
    {
        $this->test->testException2();
    }

    /**
     * @expectedException \Stub\Exception
     * @expectedExceptionMessage hello3
     */
    public function testException3()
    {
        $this->test->testException3();
    }

    /**
     * @expectedException \Stub\Exception
     * @expectedExceptionMessage hello4
     */
    public function testException4()
    {
        $this->test->testException4();
    }

    /**
     * @expectedException \Stub\Exception
     * @expectedExceptionMessage hello5
     */
    public function testException5()
    {
        $this->test->testException5();
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Test
     */
    public function testExceptionString()
    {
        $this->test->testExceptionLiteral('string');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage t
     */
    public function testExceptionChar()
    {
        $this->test->testExceptionLiteral('char');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage 123
     */
    public function testExceptionInt()
    {
        $this->test->testExceptionLiteral('int');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage 123.123
     */
    public function testExceptionDouble()
    {
        $this->test->testExceptionLiteral('double');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Hello, Zephir
     */
    public function testExceptionSprintf()
    {
        $this->test->testExceptionSprintf('Zephir');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Framework Phalcon written using Zephir
     */
    public function testExceptionConcat()
    {
        $this->test->testExceptionConcat('Phalcon', 'Zephir');
    }

    public function testExceptionRethrow()
    {
        try {
            $this->test->testExceptionRethrow();
            $this->assertFalse();
        } catch (\Exception $e) {
            $this->assertSame(11, $e->getLine());
        }
    }

    public function testMultiException()
    {
        try {
            $this->test->testMultiException('test', new Exception('Some Exception'));
        } catch (Exception $e) {
            $this->assertSame($e->getMessage(), 'Some Exception');
        }
        $this->test->internalExceptionCallable = function () {
            return false;
        };
        try {
            $value = $this->test->testMultiException('test', new Exception('Some Exception'));
            $this->assertSame($value, 'test');
        } catch (Exception $e) {
            $this->assertTrue(false);
        }
        try {
            $this->test->testMultiException('test', new \Exception('Some Exception'));
        } catch (\Exception $e) {
            $this->assertSame($e->getMessage(), 'Some Exception');
        }
        $this->test->exceptionCallable = function () {
            return false;
        };
        try {
            $this->test->testMultiException('test', new \Exception('Some Exception'));
            $this->assertSame($value, 'test');
        } catch (\Exception $e) {
            $this->assertTrue(false);
        }
    }

    public function testIssue1325()
    {
        $res = $this->test->issue1325();
        $this->assertSame(1, $res);
    }
}
