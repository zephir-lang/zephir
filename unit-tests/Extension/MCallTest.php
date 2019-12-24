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
use Test\Mcall;

class MCallTest extends TestCase
{
    /**
     * @var \ReflectionClass
     */
    private $reflection;

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
        $t = new Mcall();

        $this->assertSame($t->testCall1(), 'hello public');
        $this->assertSame($t->testCall2(), 'hello protected');
        $this->assertSame($t->testCall3(), 'hello private');

        $this->assertSame($t->testCall4(2, 3), 5);
        $this->assertSame($t->testCall5(6, 7), 13);
        $this->assertSame($t->testCall6(4, 5), 9);

        $this->assertSame($t->testCall7(2, 3), 10);
        $this->assertSame($t->testCall8(6, 7), 26);
        $this->assertSame($t->testCall9(4, 5), 18);

        $this->assertSame($t->testCall10(), 'hello public');
        $this->assertSame($t->testCall11(), 'hello protected');
        $this->assertSame($t->testCall12(), 'hello private');

        $this->assertSame($t->testCall13(2, 3), 5);
        $this->assertSame($t->testCall14(6, 7), 13);
        $this->assertSame($t->testCall15(4, 5), 9);
    }

    public function testOptionalParameterString()
    {
        $t = new Mcall();
        $this->assertSame($t->optionalParameterString('test'), 'test');
        $this->assertSame($t->optionalParameterString(), 'test string');
        $this->assertSame($t->optionalParameterStringNull(), '');
    }

    public function testOptionalParameterInt()
    {
        $t = new Mcall();
        $this->assertSame($t->optionalParameterInt(1), 1);
        $this->assertSame($t->optionalParameterInt(), 2);
    }

    public function testOptionalParameterVar()
    {
        $t = new Mcall();
        $this->assertSame($t->optionalParameterVar(1), 1);
        $this->assertSame($t->optionalParameterVar('testtesttesttest'), 'testtesttesttest');
        $this->assertNull($t->optionalParameterVar());
    }

    public function testOptionalParameterBoolean()
    {
        $t = new Mcall();
        $this->assertFalse($t->optionalParameterBoolFalse());
        $this->assertTrue($t->optionalParameterBoolTrue());

        $this->assertFalse($t->optionalParameterBooleanNull());
        $this->assertTrue($t->optionalParameterBooleanNull(true));

        $this->assertTrue($t->optionalParameterBoolean());
        $this->assertFalse($t->optionalParameterBoolean(false));
    }

    /**
     * @test
     */
    public function arrayParamWithDefaultEmptyArray()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($t->arrayParamWithDefaultEmptyArray(), []);
        $this->assertSame($t->arrayParamWithDefaultEmptyArray([1]), [1]);
    }

    /**
     * @test
     */
    public function arrayParamWithDefaultNullValue()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($t->arrayParamWithDefaultNullValue(), []);
        $this->assertSame($t->arrayParamWithDefaultNullValue([1]), [1]);
    }

    /**
     * @test
     */
    public function arrayParam()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertTrue($this->getMethodFirstParameter()->isArray());
        $this->assertSame($t->arrayParam([]), []);
        $this->assertSame($t->arrayParam([1, 2, 3]), [1, 2, 3]);
    }

    /**
     * @test
     */
    public function objectParamCastStdClass()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('stdClass', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('stdClass', $t->objectParamCastStdClass(new \stdClass()));
    }

    /**
     * @test
     */
    public function objectParamCastOoParam()
    {
        $t = new Mcall();

        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('Test\Oo\Param', $this->getMethodFirstParameter()->getClass()->getName());
        $this->assertInstanceOf('Test\Oo\Param', $t->objectParamCastOoParam(new \Test\Oo\Param()));
    }

    /**
     * @return \ReflectionParameter
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
            return $this->reflection = new \ReflectionClass('\Test\Mcall');
        }

        return $this->reflection;
    }
}
