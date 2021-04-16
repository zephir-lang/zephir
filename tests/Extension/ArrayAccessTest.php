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

final class ArrayAccessTest extends TestCase
{
    public function testTest(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertTrue($class->exits('one'));
        $this->assertSame(2, $class->get());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/645
     */
    public function testIssue645(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame([], $class->issue645());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1155
     */
    public function testIssue1155(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertFalse($class->issue1155());
    }

    public function testIssue1094(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertFalse($class->issue1094Test1());
        $this->assertFalse($class->issue1094Test1([]));
        $this->assertFalse($class->issue1094Test1(['test' => 'ok']));

        $this->assertTrue($class->issue1094Test2());
        $this->assertFalse($class->issue1094Test2([]));
        $this->assertFalse($class->issue1094Test2(['test' => 'ok']));

        $this->assertTrue($class->issue1094Test3());
        $this->assertFalse($class->issue1094Test3([]));
        $this->assertFalse($class->issue1094Test3(['test' => 'ok']));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public function testIssue1086StaticallyCalledFunctionWithArrayAsArgMustReturnArray(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $actual = $class->issue1086WontNullArrayAfterPassViaStaticWithStrictParams();
        $this->assertSame(['test' => 123], $actual);

        $actual = $class->issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams();
        $this->assertSame(['test' => 123], $actual);
    }
}
