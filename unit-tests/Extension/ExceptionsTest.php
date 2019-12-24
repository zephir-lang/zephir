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
use Test\Exception;
use Test\Exceptions;

class ExceptionsTest extends TestCase
{
    /**
     * @expectedException \Test\Exception
     * @expectedExceptionMessage hello1
     */
    public function testException1()
    {
        $t = new Exceptions();
        $t->testException1();
    }

    /**
     * @expectedException \Test\Exception
     * @expectedExceptionMessage hello2
     */
    public function testException2()
    {
        $t = new Exceptions();
        $t->testException2();
    }

    /**
     * @expectedException \Test\Exception
     * @expectedExceptionMessage hello3
     */
    public function testException3()
    {
        $t = new Exceptions();
        $t->testException3();
    }

    /**
     * @expectedException \Test\Exception
     * @expectedExceptionMessage hello4
     */
    public function testException4()
    {
        $t = new Exceptions();
        $t->testException4();
    }

    /**
     * @expectedException \Test\Exception
     * @expectedExceptionMessage hello5
     */
    public function testException5()
    {
        $t = new Exceptions();
        $t->testException5();
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Test
     */
    public function testExceptionString()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('string');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage t
     */
    public function testExceptionChar()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('char');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage 123
     */
    public function testExceptionInt()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('int');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage 123.123
     */
    public function testExceptionDouble()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('double');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Hello, Zephir
     */
    public function testExceptionSprintf()
    {
        $t = new Exceptions();
        $t->testExceptionSprintf('Zephir');
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Framework Phalcon written using Zephir
     */
    public function testExceptionConcat()
    {
        $t = new Exceptions();
        $t->testExceptionConcat('Phalcon', 'Zephir');
    }

    public function testExceptionRethrow()
    {
        $t = new Exceptions();
        try {
            $t->testExceptionRethrow();
            $this->assertFalse();
        } catch (\Exception $e) {
            $this->assertSame(11, $e->getLine());
        }
    }

    public function testMultiException()
    {
        $t = new Exceptions();
        try {
            $t->testMultiException('test', new Exception('Some Exception'));
        } catch (Exception $e) {
            $this->assertSame($e->getMessage(), 'Some Exception');
        }
        $t->internalExceptionCallable = function () {
            return false;
        };
        try {
            $value = $t->testMultiException('test', new Exception('Some Exception'));
            $this->assertSame($value, 'test');
        } catch (Exception $e) {
            $this->assertTrue(false);
        }
        try {
            $t->testMultiException('test', new \Exception('Some Exception'));
        } catch (\Exception $e) {
            $this->assertSame($e->getMessage(), 'Some Exception');
        }
        $t->exceptionCallable = function () {
            return false;
        };
        try {
            $t->testMultiException('test', new \Exception('Some Exception'));
            $this->assertSame($value, 'test');
        } catch (\Exception $e) {
            $this->assertTrue(false);
        }
    }

    public function testIssue1325()
    {
        $t = new Exceptions();
        $res = $t->issue1325();
        $this->assertSame(1, $res);
    }
}
