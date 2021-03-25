<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Exception;
use Stub\Exceptions;

final class ExceptionsTest extends TestCase
{
    private Exceptions $test;

    protected function setUp(): void
    {
        $this->test = new Exceptions();
    }

    public function testException1(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('hello1');

        $this->test->testException1();
    }

    public function testException2(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('hello2');

        $this->test->testException2();
    }

    public function testException3(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('hello3');

        $this->test->testException3();
    }

    public function testException4(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('hello4');

        $this->test->testException4();
    }

    public function testException5(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('hello5');

        $this->test->testException5();
    }

    public function testExceptionString(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Test');

        $this->test->testExceptionLiteral('string');
    }

    public function testExceptionChar(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('t');

        $this->test->testExceptionLiteral('char');
    }

    public function testExceptionInt(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage(123);

        $this->test->testExceptionLiteral('int');
    }

    public function testExceptionDouble(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage(123.123);

        $this->test->testExceptionLiteral('double');
    }

    public function testExceptionSprintf(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Hello, Zephir');

        $this->test->testExceptionSprintf('Zephir');
    }

    public function testExceptionConcat(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Framework Phalcon written using Zephir');

        $this->test->testExceptionConcat('Phalcon', 'Zephir');
    }

    public function testExceptionRethrow(): void
    {
        try {
            $this->test->testExceptionRethrow();
            $this->assertFalse();
        } catch (\Exception $e) {
            $this->assertSame(11, $e->getLine());
        }
    }

    public function testMultiException(): void
    {
        try {
            $this->test->testMultiException('test', new Exception('Some Exception'));
        } catch (\Exception $e) {
            $this->assertSame($e->getMessage(), 'Some Exception');
        }

        $this->test->internalExceptionCallable = function () {
            return false;
        };

        try {
            $value = $this->test->testMultiException('test', new Exception('Some Exception'));
            $this->assertSame($value, 'test');
        } catch (\Exception $e) {
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

    public function testIssue1325(): void
    {
        $res = $this->test->issue1325();
        $this->assertSame(1, $res);
    }
}
