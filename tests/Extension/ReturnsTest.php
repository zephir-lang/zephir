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
use Stub\Returns;

class ReturnsTest extends TestCase
{
    /**
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function testShouldReturnWithParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithParameter('Hello World!'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function testShouldReturnWithParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithParameter('Hello World!'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function testShouldReturnWithoutParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithoutParameter());
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1534
     */
    public function testShouldReturnWithoutParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithoutParameter());
    }
}
