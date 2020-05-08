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
use Test\Returns;

class ReturnsTest extends TestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function shouldReturnWithParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithParameter('Hello World!'));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function shouldReturnWithParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithParameter('Hello World!'));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function shouldReturnWithoutParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithoutParameter());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function shouldReturnWithoutParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithoutParameter());
    }
}
