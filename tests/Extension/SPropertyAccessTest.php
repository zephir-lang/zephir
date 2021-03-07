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
use Stub\SPropertyAccess;

final class SPropertyAccessTest extends TestCase
{
    public function testStaticPropertyAccess(): void
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgument(','), ',');
        $this->assertSame($spa->testArgument(), '.');
    }

    public function testStaticPropertyAccessWithUnderscore(): void
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgumentWithUnderscore(','), ',');
        $this->assertSame($spa->testArgumentWithUnderscore(), '.');
    }

    public function testShouldMutateStringVarInsideCycle(): void
    {
        $this->assertEquals(
            str_repeat(' + ', 4),
            SPropertyAccess::mutateStringVarInsideCycle()
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1494
     */
    public function testShouldMutateIntVarInsideCycle(): void
    {
        $this->assertEquals(
            42,
            SPropertyAccess::mutateIntVarInsideCycle()
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1494
     */
    public function testShouldMutateDoubleVarInsideCycle(): void
    {
        $this->assertEquals(
            3.14,
            SPropertyAccess::mutateDoubleVarInsideCycle()
        );
    }

    public function testShouldMutateArrayVarInsideCycle(): void
    {
        $this->assertEquals(
            [0, 1, 2, 3],
            SPropertyAccess::mutateArrayVarInsideCycle()
        );
    }
}
