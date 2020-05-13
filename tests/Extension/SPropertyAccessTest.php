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

    /**
     * @test
     */
    public function shouldMutateStringVarInsideCycle()
    {
        $this->assertEquals(
            str_repeat(' + ', 4),
            SPropertyAccess::mutateStringVarInsideCycle()
        );
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1494
     */
    public function shouldMutateIntVarInsideCycle()
    {
        $this->assertEquals(
            42,
            SPropertyAccess::mutateIntVarInsideCycle()
        );
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1494
     */
    public function shouldMutateDoubleVarInsideCycle()
    {
        $this->assertEquals(
            3.14,
            SPropertyAccess::mutateDoubleVarInsideCycle()
        );
    }

    /**
     * @test
     */
    public function shouldMutateArrayVarInsideCycle()
    {
        $this->assertEquals(
            [0, 1, 2, 3],
            SPropertyAccess::mutateArrayVarInsideCycle()
        );
    }
}
