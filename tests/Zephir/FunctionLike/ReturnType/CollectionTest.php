<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\FunctionLike\ReturnType;

use PHPUnit\Framework\TestCase;
use Zephir\FunctionLike\ReturnType\CastHint;
use Zephir\FunctionLike\ReturnType\Collection;
use Zephir\FunctionLike\ReturnType\RealType;
use Zephir\Types;

class CollectionTest extends TestCase
{
    /**
     * @test
     * @expectedException \Zephir\Exception\InvalidArgumentException
     */
    public function shouldThrowExceptionOnInvalidAttachedObject()
    {
        $this->expectExceptionMessage(
            'Expecting an instance of Zephir\FunctionLike\ReturnType\TypeInterface, got instance of stdClass'
        );

        $collection = new Collection();
        $collection->attach(new \stdClass());
    }

    /**
     * @test
     * @expectedException \Zephir\Exception\InvalidArgumentException
     * @expectedExceptionMessage Expecting an instance of Zephir\FunctionLike\ReturnType\TypeInterface, got array
     */
    public function shouldThrowExceptionOnInvalidAttachedValue()
    {
        $collection = new Collection();
        $collection->attach([]);
    }

    /** @test */
    public function shouldAttachTypes()
    {
        $collection = new Collection();

        $collection->attach(new RealType(Types::T_INT));
        $this->assertCount(1, $collection);

        $collection->attach(new RealType(Types::T_INT));
        $this->assertCount(1, $collection);

        $collection->attach(new RealType(Types::T_STRING));
        $this->assertCount(2, $collection);

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertCount(3, $collection);

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertCount(4, $collection);

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class2'));
        $this->assertCount(4, $collection);
    }

    /** @test */
    public function shouldDetectOnlyVoid()
    {
        $collection = new Collection();

        $this->assertFalse($collection->onlyVoid());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertTrue($collection->onlyVoid());

        $collection->attach(new RealType(Types::T_INT));
        $this->assertFalse($collection->onlyVoid());
    }

    /** @test */
    public function shouldCheckIfTheCollectionHasRealTypeOrCastHints()
    {
        $collection = new Collection();
        $this->assertFalse($collection->hasReturnTypes());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->hasReturnTypes());

        $collection->attach(new RealType(Types::T_INT));
        $this->assertTrue($collection->hasReturnTypes());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertTrue($collection->hasReturnTypes());
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreNullCompatible()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesNullCompatible());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->areReturnTypesNullCompatible());

        $collection->attach(new RealType(Types::T_NULL));
        $this->assertTrue($collection->areReturnTypesNullCompatible());

        $collection = new Collection();
        $collection->attach(new RealType(Types::T_NULL));
        $this->assertTrue($collection->areReturnTypesNullCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesNullCompatible());
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreSpecial()
    {
        $collection = new Collection();
        $this->assertFalse($collection->onlySpecial());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->onlySpecial());

        $collection->attach(new CastHint(Types::T_NUMBER, null));
        $this->assertFalse($collection->onlySpecial());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_NUMBER, null));
        $this->assertTrue($collection->onlySpecial());

        $collection->attach(new CastHint(Types::T_RESOURCE, null));
        $collection->attach(new CastHint(Types::T_VARIABLE, null));
        $collection->attach(new CastHint(Types::T_CALLABLE, null));
        $collection->attach(new CastHint(Types::T_ITERABLE, null));
        $this->assertTrue($collection->onlySpecial());

        $collection->attach(new CastHint(Types::T_OBJECT, null));
        $this->assertFalse($collection->onlySpecial());
    }

    /**
     * @test
     * @dataProvider intCompatibleProvider
     *
     * @param string $type
     * @param bool   $expected
     */
    public function shouldDetectWhetherReturnTypesAreIntCompatible($type, $expected)
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesIntCompatible());

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesIntCompatible());

        $collection->attach(new RealType($type));
        $this->assertTrue($expected === $collection->areReturnTypesIntCompatible());
    }

    public function intCompatibleProvider()
    {
        return [
            [Types::T_INT, true],
            [Types::T_UINT, true],
            [Types::T_CHAR, true],
            [Types::T_UCHAR, true],
            [Types::T_LONG, true],
            [Types::T_ULONG, true],
            [Types::T_DOUBLE, false],
            [Types::T_FLOAT, false],
            [Types::T_NUMBER, false],
            [Types::T_NULL, false],
            [Types::T_BOOL, false],
            [Types::T_STRING, false],
            [Types::T_ISTRING, false],
            [Types::T_VOID, false],
            [Types::T_VARIABLE, false],
            [Types::T_MIXED, false],
            [Types::T_ARRAY, false],
            [Types::T_OBJECT, false],
            [Types::T_CALLABLE, false],
            [Types::T_ITERABLE, false],
            [Types::T_RESOURCE, false],
            [Types::T_UNDEFINED, false],
        ];
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreDoubleCompatible()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesDoubleCompatible());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->areReturnTypesDoubleCompatible());

        $collection->attach(new RealType(Types::T_DOUBLE));
        $this->assertTrue($collection->areReturnTypesDoubleCompatible());

        $collection = new Collection();
        $collection->attach(new RealType(Types::T_DOUBLE));
        $this->assertTrue($collection->areReturnTypesDoubleCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesDoubleCompatible());
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreBoolCompatible()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesBoolCompatible());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->areReturnTypesBoolCompatible());

        $collection->attach(new RealType(Types::T_BOOL));
        $this->assertTrue($collection->areReturnTypesBoolCompatible());

        $collection = new Collection();
        $collection->attach(new RealType(Types::T_BOOL));
        $this->assertTrue($collection->areReturnTypesBoolCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesBoolCompatible());
    }

    /**
     * @test
     * @dataProvider stringCompatibleProvider
     *
     * @param string $type
     * @param bool   $expected
     */
    public function shouldDetectWhetherReturnTypesAreStringCompatible($type, $expected)
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesStringCompatible());

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesStringCompatible());

        $collection->attach(new RealType($type));
        $this->assertTrue($expected === $collection->areReturnTypesStringCompatible());
    }

    public function stringCompatibleProvider()
    {
        return [
            [Types::T_INT, false],
            [Types::T_UINT, false],
            [Types::T_CHAR, false],
            [Types::T_UCHAR, false],
            [Types::T_LONG, false],
            [Types::T_ULONG, false],
            [Types::T_DOUBLE, false],
            [Types::T_FLOAT, false],
            [Types::T_NULL, false],
            [Types::T_NUMBER, false],
            [Types::T_BOOL, false],
            [Types::T_STRING, true],
            [Types::T_ISTRING, true],
            [Types::T_VOID, false],
            [Types::T_VARIABLE, false],
            [Types::T_MIXED, false],
            [Types::T_ARRAY, false],
            [Types::T_OBJECT, false],
            [Types::T_CALLABLE, false],
            [Types::T_ITERABLE, false],
            [Types::T_RESOURCE, false],
            [Types::T_UNDEFINED, false],
        ];
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreArrayCompatible()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesArrayCompatible());

        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->areReturnTypesArrayCompatible());

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertTrue($collection->areReturnTypesArrayCompatible());

        $collection = new Collection();
        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertTrue($collection->areReturnTypesArrayCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1', null, true));
        $this->assertTrue($collection->areReturnTypesArrayCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertFalse($collection->areReturnTypesArrayCompatible());

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1', null, true));
        $this->assertFalse($collection->areReturnTypesArrayCompatible());

        $collection->attach(new CastHint(Types::T_UNDEFINED, 'Class2', null, false));
        $this->assertFalse($collection->areReturnTypesArrayCompatible());

        $collection->attach(new CastHint(Types::T_UNDEFINED, 'Class3', null, true));
        $this->assertFalse($collection->areReturnTypesArrayCompatible());
    }

    /** @test */
    public function shouldDetectWhetherReturnTypesAreObjectCompatible()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesObjectCompatible());

        $collection->attach(new CastHint(Types::T_UNDEFINED, 'Class1'));
        $this->assertFalse($collection->areReturnTypesObjectCompatible());

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class2'));
        $this->assertTrue($collection->areReturnTypesObjectCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertTrue($collection->areReturnTypesObjectCompatible());

        $collection = new Collection();
        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1', null, true));
        $this->assertFalse($collection->areReturnTypesObjectCompatible());

        $collection = new Collection();
        $collection->attach(new RealType(Types::T_VOID));
        $this->assertFalse($collection->areReturnTypesObjectCompatible());
    }

    /**
     * @test
     */
    public function shouldDetectWellKnownTypes()
    {
        $collection = new Collection();
        $this->assertFalse($collection->areReturnTypesWellKnown());

        $wellKnown = [
            Types::T_INT,
            Types::T_UINT,
            Types::T_CHAR,
            Types::T_UCHAR,
            Types::T_LONG,
            Types::T_ULONG,
            Types::T_DOUBLE,
            Types::T_NULL,
            Types::T_BOOL,
            Types::T_STRING,
            Types::T_ISTRING,
            Types::T_VOID,
            Types::T_ARRAY,
        ];

        foreach ($wellKnown as $type) {
            $collection->attach(new RealType($type));
        }

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertTrue($collection->areReturnTypesWellKnown());

        $collection->attach(new RealType(Types::T_FLOAT));
        $this->assertFalse($collection->areReturnTypesWellKnown());

        $otherTypes = [
            Types::T_FLOAT,
            Types::T_NUMBER,
            Types::T_VARIABLE,
            Types::T_MIXED,
            Types::T_CALLABLE,
            Types::T_ITERABLE,
            Types::T_RESOURCE,
            Types::T_UNDEFINED,
        ];

        $collection = new Collection();

        foreach ($otherTypes as $type) {
            $collection->attach(new RealType($type));
        }

        $this->assertFalse($collection->areReturnTypesWellKnown());

        foreach ($wellKnown as $type) {
            $collection->attach(new RealType($type));
        }

        $this->assertFalse($collection->areReturnTypesWellKnown());
    }

    /**
     * @test
     * @dataProvider compatibleTypesProvider
     *
     * @param string $type
     */
    public function shouldDetectCompatibleTypesForTrivialCases($type)
    {
        $collection = new Collection();
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType($type));
        $this->assertTrue($collection->areReturnTypesCompatible());
    }

    public function compatibleTypesProvider()
    {
        return [
            [Types::T_INT],
            [Types::T_UINT],
            [Types::T_CHAR],
            [Types::T_UCHAR],
            [Types::T_LONG],
            [Types::T_ULONG],
            [Types::T_DOUBLE],
            [Types::T_FLOAT],
            [Types::T_NULL],
            [Types::T_NUMBER],
            [Types::T_BOOL],
            [Types::T_STRING],
            [Types::T_ISTRING],
            [Types::T_VOID],
            [Types::T_VARIABLE],
            [Types::T_MIXED],
            [Types::T_ARRAY],
            [Types::T_OBJECT],
            [Types::T_CALLABLE],
            [Types::T_ITERABLE],
            [Types::T_RESOURCE],
            [Types::T_UNDEFINED],
        ];
    }

    /** @test */
    public function shouldDetectCompatibleTypesForObjectLike()
    {
        $collection = new Collection();

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new CastHint(Types::T_RESOURCE, null, null, true));
        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForCollections()
    {
        $collection = new Collection();

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1', null, true));
        $collection->attach(new RealType(Types::T_ARRAY));

        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection = new Collection();

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $collection->attach(new RealType(Types::T_ARRAY));

        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForIntegers()
    {
        $collection = new Collection();

        $collection->attach(new RealType(Types::T_CHAR));
        $collection->attach(new RealType(Types::T_UCHAR));
        $collection->attach(new RealType(Types::T_INT));
        $collection->attach(new RealType(Types::T_UINT));
        $collection->attach(new RealType(Types::T_LONG));
        $collection->attach(new RealType(Types::T_ULONG));

        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForBool()
    {
        $collection = new Collection();

        $collection->attach(new RealType(Types::T_BOOL));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_CHAR));
        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForString()
    {
        $collection = new Collection();

        $collection->attach(new RealType(Types::T_STRING));
        $collection->attach(new RealType(Types::T_ISTRING));

        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_BOOL));
        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForDouble()
    {
        $collection = new Collection();

        $collection->attach(new RealType(Types::T_DOUBLE));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_STRING));
        $this->assertFalse($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldDetectCompatibleTypesForKnownTypes()
    {
        $collection = new Collection();

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_NULL));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertFalse($collection->areReturnTypesCompatible());

        $collection = new Collection();

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new RealType(Types::T_CHAR));
        $collection->attach(new RealType(Types::T_BOOL));
        $collection->attach(new RealType(Types::T_STRING));
        $collection->attach(new RealType(Types::T_DOUBLE));

        $this->assertFalse($collection->areReturnTypesCompatible());

        $collection = new Collection();

        $collection->attach(new RealType(Types::T_ISTRING));
        $collection->attach(new RealType(Types::T_NULL));

        $this->assertTrue($collection->areReturnTypesCompatible());

        $collection->attach(new CastHint(Types::T_RESOURCE, null));
        $this->assertFalse($collection->areReturnTypesCompatible());

        $collection = new Collection();

        $collection->attach(new RealType(Types::T_NULL));
        $this->assertTrue($collection->areReturnTypesCompatible());
    }

    /** @test */
    public function shouldGetOnlyRealTypes()
    {
        $collection = new Collection();
        $this->assertCount(0, $collection->getRealReturnTypes());

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertCount(1, $collection->getRealReturnTypes());

        $collection->attach(new RealType(Types::T_CHAR));
        $collection->attach(new RealType(Types::T_BOOL));
        $collection->attach(new RealType(Types::T_STRING));
        $collection->attach(new RealType(Types::T_DOUBLE));
        $this->assertCount(5, $collection->getRealReturnTypes());

        $collection->attach(new CastHint(Types::T_RESOURCE, null));
        $this->assertCount(5, $collection->getRealReturnTypes());
    }

    /** @test */
    public function shouldGetOnlyCastHint()
    {
        $collection = new Collection();
        $this->assertCount(0, $collection->getCastHintedReturnTypes());

        $collection->attach(new CastHint(Types::T_RESOURCE, null));
        $this->assertCount(1, $collection->getCastHintedReturnTypes());

        $collection->attach(new RealType(Types::T_ARRAY));
        $this->assertCount(1, $collection->getCastHintedReturnTypes());

        $collection->attach(new CastHint(Types::T_OBJECT, 'Class1'));
        $collection->attach(new CastHint(Types::T_CALLABLE, null, null, true));
        $this->assertCount(3, $collection->getCastHintedReturnTypes());
    }
}
