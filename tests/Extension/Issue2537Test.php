<?php

declare(strict_types=1);

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
use ReflectionMethod;
use ReflectionNamedType;
use Stub\Issue2537;
use Stub\Issue2537Child;

/**
 * Regression coverage for PR #2537.
 *
 * Background. The original `#2505` fix lowercased `<self>`, `<static>`,
 * and `<parent>` and emitted them verbatim inside
 * `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(..., <keyword>, 0)`. PHP treats
 * the third argument of that macro as a literal class name and resolves
 * it via the arginfo class-name resolver at `MINIT`. For `self` and
 * `parent` that resolution succeeds because PHP recognizes the reserved
 * names. For `static` there is no class entry, and the resolution aborts
 * with:
 *
 *     Fatal error: static must be registered before <Class>
 *
 * The crash only surfaces when a subclass *overrides* a `-> <static>`
 * method, because that is when PHP actually runs the resolver against
 * the inherited arginfo. Issue2505Test only covered the no-override
 * path, which is why this regression made it through the 0.21.0
 * release. The fix emits `ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX`
 * with the `MAY_BE_STATIC` mask, which the engine recognizes as a
 * built-in late-static-binding marker, requiring no class lookup.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
final class Issue2537Test extends TestCase
{
    public function testExtensionLoadsWithStaticReturnOverride(): void
    {
        $this->assertTrue(
            class_exists(Issue2537Child::class, false),
            'Stub\Issue2537Child must be registered. If MINIT failed with '
            . '"static must be registered before Stub\\\\Issue2537Child" the '
            . 'extension would never have loaded, and the autoload-disabled '
            . 'class_exists() check would return false here.'
        );
    }

    public function testReflectionReportsStaticOnParentMethod(): void
    {
        $rt = (new ReflectionMethod(Issue2537::class, 'make'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('static', $rt->getName());
        $this->assertFalse($rt->allowsNull());
    }

    public function testReflectionReportsStaticOnChildOverride(): void
    {
        $rt = (new ReflectionMethod(Issue2537Child::class, 'make'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('static', $rt->getName());
        $this->assertFalse($rt->allowsNull());
    }

    public function testReflectionReportsNullableStaticOnParent(): void
    {
        $rt = (new ReflectionMethod(Issue2537::class, 'makeNullable'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('static', $rt->getName());
        $this->assertTrue(
            $rt->allowsNull(),
            'The MAY_BE_NULL bit must be ORed into the type mask so PHP reports '
            . 'the return as nullable.'
        );
    }

    public function testReflectionReportsNullableStaticOnChildOverride(): void
    {
        $rt = (new ReflectionMethod(Issue2537Child::class, 'makeNullable'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('static', $rt->getName());
        $this->assertTrue($rt->allowsNull());
    }

    public function testReflectionReportsParentOnChildOnly(): void
    {
        $rt = (new ReflectionMethod(Issue2537Child::class, 'makeViaParent'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('parent', $rt->getName(), 'parent must reach the engine as a reserved keyword');
    }

    public function testReflectionReportsSelfOnChild(): void
    {
        $rt = (new ReflectionMethod(Issue2537Child::class, 'makeViaSelf'))->getReturnType();

        $this->assertInstanceOf(ReflectionNamedType::class, $rt);
        $this->assertSame('self', $rt->getName());
    }

    public function testLateStaticBindingReturnsBaseClassFromBase(): void
    {
        $sut = new Issue2537();

        $this->assertSame(Issue2537::class, get_class($sut->make()));
        $this->assertSame('parent', $sut->getTag());
    }

    public function testLateStaticBindingReturnsChildClassFromChild(): void
    {
        $sut = new Issue2537Child();

        $this->assertSame(Issue2537Child::class, get_class($sut->make()));
        $this->assertSame('child', $sut->getTag());
    }

    public function testNullableStaticAcceptsObjectReturn(): void
    {
        $sut = new Issue2537Child();

        $this->assertInstanceOf(Issue2537Child::class, $sut->makeNullable());
    }

    public function testParentReturnFromChildYieldsParentInstance(): void
    {
        $sut = new Issue2537Child();
        $out = $sut->makeViaParent();

        $this->assertInstanceOf(Issue2537::class, $out);
        $this->assertNotInstanceOf(Issue2537Child::class, $out);
    }
}
