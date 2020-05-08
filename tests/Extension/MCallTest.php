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
use Stub\Mcall;

class MCallTest extends TestCase
{
    /** @var \ReflectionClass */
    private $reflection;

    /** @var Mcall */
    private $test;

    public function setUp()
    {
        $this->test = new Mcall();
    }

    public function assertNumberOfParameters($number)
    {
        $this->assertSame($number, $this->getReflection()->getMethod($this->getName())->getNumberOfParameters());
    }

    public function assertNumberOfRequiredParameters($number)
    {
        $this->assertSame($number, $this->getReflection()->getMethod($this->getName())->getNumberOfRequiredParameters());
    }

    public function testCall()
    {
        $this->assertSame($this->test->testCall1(), 'hello public');
        $this->assertSame($this->test->testCall2(), 'hello protected');
        $this->assertSame($this->test->testCall3(), 'hello private');

        $this->assertSame($this->test->testCall4(2, 3), 5);
        $this->assertSame($this->test->testCall5(6, 7), 13);
        $this->assertSame($this->test->testCall6(4, 5), 9);

        $this->assertSame($this->test->testCall7(2, 3), 10);
        $this->assertSame($this->test->testCall8(6, 7), 26);
        $this->assertSame($this->test->testCall9(4, 5), 18);

        $this->assertSame($this->test->testCall10(), 'hello public');
        $this->assertSame($this->test->testCall11(), 'hello protected');
        $this->assertSame($this->test->testCall12(), 'hello private');

        $this->assertSame($this->test->testCall13(2, 3), 5);
        $this->assertSame($this->test->testCall14(6, 7), 13);
        $this->assertSame($this->test->testCall15(4, 5), 9);
    }

    public function testOptionalParameterString()
    {
        $this->assertSame($this->test->optionalParameterString('test'), 'test');
        $this->assertSame($this->test->optionalParameterString(), 'test string');
        $this->assertSame($this->test->optionalParameterStringNull(), '');
    }

    public function testOptionalParameterInt()
    {
        $this->assertSame($this->test->optionalParameterInt(1), 1);
        $this->assertSame($this->test->optionalParameterInt(), 2);
    }

    public function testOptionalParameterVar()
    {
        $this->assertSame($this->test->optionalParameterVar(1), 1);
        $this->assertSame($this->test->optionalParameterVar('testtesttesttest'), 'testtesttesttest');
        $this->assertNull($this->test->optionalParameterVar());
    }

    public function testOptionalParameterBoolean()
    {
        $this->assertFalse($this->test->optionalParameterBoolFalse());
        $this->assertTrue($this->test->optionalParameterBoolTrue());

        $this->assertFalse($this->test->optionalParameterBooleanNull());
        $this->assertTrue($this->test->optionalParameterBooleanNull(true));

        $this->assertTrue($this->test->optionalParameterBoolean());
        $this->assertFalse($this->test->optionalParameterBoolean(false));
    }

    /** @test */
    public function arrayParamWithDefaultEmptyArray()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->arrayParamWithDefaultEmptyArray(), []);
        $this->assertSame($this->test->arrayParamWithDefaultEmptyArray([1]), [1]);
    }

    /** @test */
    public function arrayParamWithDefaultNullValue()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->arrayParamWithDefaultNullValue(), []);
        $this->assertSame($this->test->arrayParamWithDefaultNullValue([1]), [1]);
    }

    /** @test */
    public function arrayParam()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->arrayParam([]), []);
        $this->assertSame($this->test->arrayParam([1, 2, 3]), [1, 2, 3]);
    }

    /**  @test */
    public function objectParamCastStdClass()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('stdClass', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('stdClass', $this->test->objectParamCastStdClass(new \stdClass()));
    }

    /** @test */
    public function objectParamCastOoParam()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('Test\Oo\Param', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('Test\Oo\Param', $this->test->objectParamCastOoParam(new \Stub\Oo\Param()));
    }

    /**
     * @return \ReflectionParameter
     *
     * @throws \ReflectionException
     */
    protected function getMethodFirstParameter()
    {
        $methodInfo = $this->reflection->getMethod($this->getName());
        $parameters = $methodInfo->getParameters();

        return $parameters[0];
    }

    private function getReflection()
    {
        if (null === $this->reflection) {
            return $this->reflection = new \ReflectionClass('\Stub\Mcall');
        }

        return $this->reflection;
    }
}
