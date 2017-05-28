<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | https://zephir-lang.com/license.html                                     |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
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
    public function shouldReturnWithoutParameter()
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithoutParameter());
    }
}
