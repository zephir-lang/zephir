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
use Stub\Issue2505;
use Stub\Issue2505Extended;

/**
 * Regression coverage for `-> <self>` and `-> <static>` return types.
 * The engine must see the reserved keywords (`self` / `static`), not a
 * namespaced literal class name. `<static>` must also do late static
 * binding at runtime.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
final class Issue2505Test extends TestCase
{
    public function testReflectionReportsSelfAsReservedName(): void
    {
        $rm = new ReflectionMethod(Issue2505::class, 'makeSelf');
        $rt = $rm->getReturnType();

        $this->assertNotNull($rt, 'makeSelf() must declare a return type');
        $this->assertSame('self', $rt->getName(), 'PHP must see the reserved `self` keyword');
    }

    public function testReflectionReportsSelfOnStaticMethod(): void
    {
        $rm = new ReflectionMethod(Issue2505::class, 'makeSelfStatic');
        $rt = $rm->getReturnType();

        $this->assertNotNull($rt);
        $this->assertSame('self', $rt->getName());
    }

    public function testReflectionReportsStaticAsReservedName(): void
    {
        $rm = new ReflectionMethod(Issue2505::class, 'makeStatic');
        $rt = $rm->getReturnType();

        $this->assertNotNull($rt);
        $this->assertSame('static', $rt->getName());
    }

    public function testReflectionReportsStaticOnStaticMethod(): void
    {
        $rm = new ReflectionMethod(Issue2505::class, 'makeStaticFromStatic');
        $rt = $rm->getReturnType();

        $this->assertNotNull($rt);
        $this->assertSame('static', $rt->getName());
    }

    public function testMakeSelfReturnsCorrectClass(): void
    {
        $sut = new Issue2505();
        $this->assertInstanceOf(Issue2505::class, $sut->makeSelf());
    }

    public function testMakeStaticOnBaseClassReturnsBaseClass(): void
    {
        $sut = new Issue2505();
        $this->assertSame(Issue2505::class, get_class($sut->makeStatic()));
    }

    public function testMakeStaticOnSubclassReturnsSubclass(): void
    {
        $sut = new Issue2505Extended();
        $this->assertSame(Issue2505Extended::class, get_class($sut->makeStatic()));
    }

    public function testMakeSelfOnSubclassStillReturnsBaseClass(): void
    {
        $sut = new Issue2505Extended();
        $this->assertSame(Issue2505::class, get_class($sut->makeSelf()));
    }
}
