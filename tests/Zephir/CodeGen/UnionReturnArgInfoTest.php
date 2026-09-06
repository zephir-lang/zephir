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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;
use Zephir\Exception\InvalidTypeException;

/**
 * Locks the arginfo macro chosen for union return types so the compiled
 * extension declares — and PHP enforces — every member of the union, the
 * same way a hand-written PHP union return type would.
 *
 *   - scalar-only unions             -> ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX
 *   - unions with >=1 named class    -> ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX
 *
 * Single, non-union return types (`int`, `?int`, `<Foo>`, `<Foo> | null`,
 * `object`, `void`, ...) must keep their existing precise INFO/OBJ_INFO forms
 * — those are asserted here too so the union work cannot regress them.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2428
 */
final class UnionReturnArgInfoTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_arginfo_union_test_', ['stub/issue2428']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testScalarUnionUsesTypeMask(): void
    {
        $h = $this->compileSingle('ScalarUnion', 'scalarunion.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ScalarUnion
{
    public function make(bool flag) -> int | string
    {
        return flag ? 1 : "x";
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_scalarunion_make, 0, 1, MAY_BE_LONG|MAY_BE_STRING)',
            $h
        );
    }

    public function testNullableScalarUnionOrsInMayBeNull(): void
    {
        $h = $this->compileSingle('ScalarUnionNull', 'scalarunionnull.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ScalarUnionNull
{
    public function make(int n) -> int | string | null
    {
        return n;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_scalarunionnull_make, 0, 1, MAY_BE_NULL|MAY_BE_LONG|MAY_BE_STRING)',
            $h
        );
    }

    public function testMultiClassUnionUsesObjTypeMask(): void
    {
        $h = $this->compileSingle('ClassUnion', 'classunion.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ClassUnion
{
    public function make(bool flag) -> <\stdClass> | <\ArrayObject>
    {
        return flag ? new \stdClass() : new \ArrayObject();
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classunion_make, 0, 1, stdClass|ArrayObject, 0)',
            $h
        );
    }

    public function testNullableMultiClassUnionPutsNullInMask(): void
    {
        $h = $this->compileSingle('ClassUnionNull', 'classunionnull.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ClassUnionNull
{
    public function make(int n) -> <\stdClass> | <\ArrayObject> | null
    {
        return n > 0 ? new \stdClass() : null;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classunionnull_make, 0, 1, stdClass|ArrayObject, MAY_BE_NULL)',
            $h
        );
    }

    public function testClassAndScalarUnionUsesObjTypeMask(): void
    {
        $h = $this->compileSingle('ClassOrScalar', 'classorscalar.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ClassOrScalar
{
    public function make(bool flag) -> <\stdClass> | int
    {
        return flag ? new \stdClass() : 5;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classorscalar_make, 0, 1, stdClass, MAY_BE_LONG)',
            $h
        );
    }

    public function testArrayAndGenericObjectUnionUsesTypeMask(): void
    {
        $h = $this->compileSingle('ArrayOrObject', 'arrayorobject.zep', <<<'ZEP'
namespace Stub\Issue2428;

class ArrayOrObject
{
    public function make(bool flag) -> array | object
    {
        return flag ? [] : new \stdClass();
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_arrayorobject_make, 0, 1, MAY_BE_ARRAY|MAY_BE_OBJECT)',
            $h
        );
    }

    /**
     * Single (non-union) return types must keep their precise existing forms.
     */
    public function testSingleTypesAreNotRegressed(): void
    {
        $h = $this->compileSingle('Singles', 'singles.zep', <<<'ZEP'
namespace Stub\Issue2428;

class Singles
{
    public function justInt() -> int { return 1; }
    public function nullableInt(bool f) -> int | null { return f ? 1 : null; }
    public function oneClass() -> <\stdClass> { return new \stdClass(); }
    public function nullableClass(bool f) -> <\stdClass> | null { return f ? new \stdClass() : null; }
    public function genericObject() -> object { return new \stdClass(); }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2428_singles_justint, 0, 0, IS_LONG, 0)',
            $h
        );
        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2428_singles_nullableint, 0, 1, IS_LONG, 1)',
            $h
        );
        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2428_singles_oneclass, 0, 0, stdClass, 0)',
            $h
        );
        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2428_singles_nullableclass, 0, 1, stdClass, 1)',
            $h
        );
        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_singles_genericobject, 0, 0, MAY_BE_OBJECT)',
            $h
        );
    }

    /**
     * A body returning a type that is not a member of the declared union must
     * fail compilation with a clean, specific error — never crash or silently
     * emit a wrong arginfo.
     */
    public function testReturnOutsideUnionFailsCompilationCleanly(): void
    {
        $this->expectException(InvalidTypeException::class);
        $this->expectExceptionMessage('not compatible with return-type hints');

        $this->compileSingle('BadReturn', 'badreturn.zep', <<<'ZEP'
namespace Stub\Issue2428;

class BadReturn
{
    public function make() -> int | string
    {
        return true;
    }
}
ZEP);
    }

    /**
     * A union containing a member the engine cannot describe (`callable`,
     * `resource`, dynamic `var`) must degrade gracefully: the method compiles
     * and simply carries no return-type arginfo — no partial/wrong mask, no
     * crash.
     */
    public function testUndeterminableUnionMemberEmitsNoReturnType(): void
    {
        $h = $this->compileSingle('Undet', 'undet.zep', <<<'ZEP'
namespace Stub\Issue2428;

class Undet
{
    public function make(callable c) -> int | callable
    {
        return c;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2428_undet_make, 0, 0, 1)',
            $h,
            'An undeterminable union must fall back to a plain ARG_INFO with no return type.'
        );
        $this->assertStringNotContainsString(
            'arginfo_stub_issue2428_undet_make, 0, 1, MAY_BE',
            $h,
            'It must NOT emit a partial type mask that drops the undeterminable member.'
        );
    }

    private function compileSingle(string $simpleClassName, string $fileName, string $zep): string
    {
        $srcRel = 'stub/issue2428/' . $fileName;

        $this->compileSource('Stub\\Issue2428\\' . $simpleClassName, $srcRel, $zep);

        return $this->generatedHeader($srcRel);
    }
}
