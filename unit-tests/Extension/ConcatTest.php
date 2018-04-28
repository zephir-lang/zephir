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

use Test\Concat;
use PHPUnit_Framework_TestCase;

class ConcatTest extends PHPUnit_Framework_TestCase
{
    /** @test */
    public function shouldConcatenateLocalVariableAndSelfVariable()
    {
        $t = new Concat();

        $this->assertSame(null, $t->getTestProperty());

        $t->testConcatBySelfProperty("test");
        $this->assertSame("test", $t->getTestProperty());

        $t->testConcatBySelfProperty(" string");
        $this->assertSame("test string", $t->getTestProperty());
    }

    /** @test */
    public function shouldAppendVariableToString()
    {
        $t = new Concat();
        $this->assertSame("appendtest", $t->testConcat1());
    }

    /** @test */
    public function shouldConcatenateVariableWithString()
    {
        $t = new Concat();
        $this->assertSame("appendotherappendother", $t->testConcat2());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1573
     */
    public function shouldConcatenateStringsSimilarToIntegersNumbers()
    {
        $t = new Concat();
        $this->assertSame("21", $t->testConcat3());
    }
}
