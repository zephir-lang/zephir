<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Mcall;
use Stub\Oo\Param;

final class MCallTest extends TestCase
{
    /**
     * @var mixed
     */
    private $reflection;
    private Mcall $test;

    protected function setUp(): void
    {
        $this->test = new Mcall();
    }

    public function assertNumberOfParameters($number): void
    {
        $this->assertSame($number, $this->getReflection()->getMethod($this->getName())->getNumberOfParameters());
    }

    public function assertNumberOfRequiredParameters($number): void
    {
        $this->assertSame($number, $this->getReflection()->getMethod($this->getName())->getNumberOfRequiredParameters());
    }

    public function testCall(): void
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

    public function testOptionalParameterString(): void
    {
        $this->assertSame($this->test->optionalParameterString('test'), 'test');
        $this->assertSame($this->test->optionalParameterString(), 'test string');
        $this->assertSame($this->test->optionalParameterStringNull(), null);
    }

    public function testOptionalParameterInt(): void
    {
        $this->assertSame($this->test->optionalParameterInt(1), 1);
        $this->assertSame($this->test->optionalParameterInt(), 2);
    }

    public function testOptionalParameterVar(): void
    {
        $this->assertSame($this->test->optionalParameterVar(1), 1);
        $this->assertSame($this->test->optionalParameterVar('testtesttesttest'), 'testtesttesttest');
        $this->assertNull($this->test->optionalParameterVar());
    }

    public function testOptionalParameterBoolean(): void
    {
        $this->assertFalse($this->test->optionalParameterBoolFalse());
        $this->assertTrue($this->test->optionalParameterBoolTrue());

        $this->assertFalse($this->test->optionalParameterBooleanNull());
        $this->assertTrue($this->test->optionalParameterBooleanNull(true));

        $this->assertTrue($this->test->optionalParameterBoolean());
        $this->assertFalse($this->test->optionalParameterBoolean(false));
    }

    public function testArrayParamWithDefaultEmptyArray(): void
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertSame('array', $this->getMethodFirstParameter()->getType()->getName());
        $this->assertSame($this->test->testArrayParamWithDefaultEmptyArray(), []);
        $this->assertSame($this->test->testArrayParamWithDefaultEmptyArray([1]), [1]);
    }

    public function testArrayParamWithDefaultNullValue(): void
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(0);

        $this->assertSame('array', $this->getMethodFirstParameter()->getType()->getName());
        $this->assertNull($this->test->testArrayParamWithDefaultNullValue());
        $this->assertSame($this->test->testArrayParamWithDefaultNullValue([1]), [1]);
    }

    public function testArrayParam(): void
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame('array', $this->getMethodFirstParameter()->getType()->getName());
        $this->assertSame($this->test->testArrayParam([]), []);
        $this->assertSame($this->test->testArrayParam([1, 2, 3]), [1, 2, 3]);
    }

    public function testObjectParamCastStdClass(): void
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame(\stdClass::class, $this->getMethodFirstParameter()->getType()->getName());
        $this->assertInstanceOf(\stdClass::class, $this->test->testObjectParamCastStdClass(new \stdClass()));
    }

    public function testObjectParamCastOoParam(): void
    {
        $this->assertNumberOfParameters(1);
        $this->assertNumberOfRequiredParameters(1);

        $this->assertSame(Param::class, $this->getMethodFirstParameter()->getType()->getName());
        $this->assertInstanceOf(Param::class, $this->test->testObjectParamCastOoParam(new Param()));
    }

    protected function getMethodFirstParameter(): \ReflectionParameter
    {
        $methodInfo = $this->reflection->getMethod($this->getName());
        $parameters = $methodInfo->getParameters();

        return $parameters[0];
    }

    /**
     * @return mixed|\ReflectionClass
     */
    private function getReflection()
    {
        if (null === $this->reflection) {
            return $this->reflection = new \ReflectionClass(Mcall::class);
        }

        return $this->reflection;
    }

    public function testShouldThrowTypeErrorForOptionalBoolean1(): void
    {
        $test = new Mcall();

        $this->expectException(\Error::class);
        $this->expectExceptionMessageRegExp(
            '/Argument 1 passed to Stub\\\Mcall::optionalParameterBoolean\(\) '.
            'must be of the type bool(ean)?, string given/'
        );

        $test->optionalParameterBoolean('test');
    }

    public function testShouldThrowTypeErrorForOptionalBoolean2(): void
    {
        $test = new Mcall();

        $this->expectException(\Error::class);
        $this->expectExceptionMessageRegExp(
            '/Argument 1 passed to Stub\\\Mcall::optionalParameterBoolean\(\) '.
            'must be of the type bool(ean)?, array given/'
        );

        $test->optionalParameterBoolean([]);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1136
     */
    public function testIssue1136(): void
    {
        $test = new Mcall();

        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->assertInstanceOf(\finfo::class, $test->issue1136());
        } else {
            $this->assertIsResource($test->issue1136());
        }
    }

    public function testIssue2245DynamicNullableArgMustBeNullableAsDefault(): void
    {
        $test = new Mcall();

        $this->assertNull($test->issue2245VarArgumentNullable());
    }
}
