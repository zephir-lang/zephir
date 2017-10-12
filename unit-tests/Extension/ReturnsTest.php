<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\Returns;

class ReturnsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @test
     * @issue 1534
     */
    public function shouldReturnWithParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithParameter('Hello World!'));
    }

    /**
     * @test
     * @issue 1534
     */
    public function shouldReturnWithParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithParameter('Hello World!'));
    }

    /**
     * @test
     * @issue 1534
     */
    public function shouldReturnWithoutParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithoutParameter());
    }

    /**
     * @test
     * @issue 1534
     */
    public function shouldReturnWithoutParameterStaticVersion()
    {
        $this->assertSame('Return back', Returns::returnWithoutParameter());
    }
}
