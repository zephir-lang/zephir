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

    protected function setUp(): void
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

    public function testArrayParamWithDefaultEmptyArray()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->testArrayParamWithDefaultEmptyArray(), []);
        $this->assertSame($this->test->testArrayParamWithDefaultEmptyArray([1]), [1]);
    }

    public function testArrayParamWithDefaultNullValue()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->testArrayParamWithDefaultNullValue(), []);
        $this->assertSame($this->test->testArrayParamWithDefaultNullValue([1]), [1]);
    }

    public function testArrayParam()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($this->test->testArrayParam([]), []);
        $this->assertSame($this->test->testArrayParam([1, 2, 3]), [1, 2, 3]);
    }

    public function testObjectParamCastStdClass()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('stdClass', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('stdClass', $this->test->testObjectParamCastStdClass(new \stdClass()));
    }

    public function testObjectParamCastOoParam()
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('Stub\Oo\Param', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('Stub\Oo\Param', $this->test->testObjectParamCastOoParam(new \Stub\Oo\Param()));
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
