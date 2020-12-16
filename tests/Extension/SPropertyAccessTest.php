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
use Stub\SPropertyAccess;

class SPropertyAccessTest extends TestCase
{
    public function testStaticPropertyAccess()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgument(','), ',');
        $this->assertSame($spa->testArgument(), '.');
    }

    public function testStaticPropertyAccessWithUnderscore()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgumentWithUnderscore(','), ',');
        $this->assertSame($spa->testArgumentWithUnderscore(), '.');
    }

    public function testShouldMutateStringVarInsideCycle()
    {
        $this->assertEquals(
            str_repeat(' + ', 4),
            SPropertyAccess::mutateStringVarInsideCycle()
        );
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1494
     */
    public function testShouldMutateIntVarInsideCycle()
    {
        $this->assertEquals(
            42,
            SPropertyAccess::mutateIntVarInsideCycle()
        );
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1494
     */
    public function testShouldMutateDoubleVarInsideCycle()
    {
        $this->assertEquals(
            3.14,
            SPropertyAccess::mutateDoubleVarInsideCycle()
        );
    }

    public function testShouldMutateArrayVarInsideCycle()
    {
        $this->assertEquals(
            [0, 1, 2, 3],
            SPropertyAccess::mutateArrayVarInsideCycle()
        );
    }
}
