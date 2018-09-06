<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: https://zephir-lang.com/license.html               |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\Scall;
use Test\Scallparent;
use PHPUnit\Framework\TestCase;

class ScallTest extends TestCase
{
    public function testScall()
    {
        $t = new Scall();

        $this->assertSame('hello public', $t->testMethod1());
        $this->assertSame('hello public', $t::testMethod1());
        $this->assertSame('hello public', Scall::testMethod1());

        $this->assertSame('hello public', $t->testCall1());
        $this->assertSame('hello protected', $t->testCall2());
        $this->assertSame('hello private', $t->testCall3());

        $this->assertSame(5, $t->testCall4(2, 3));
        $this->assertSame(13, $t->testCall5(6, 7));
        $this->assertSame(9, $t->testCall6(4, 5));

        $this->assertSame('hello public', $t->testCall7());
        $this->assertSame('hello protected', $t->testCall8());
        $this->assertSame('hello private', $t->testCall9());

        $this->assertSame(5, $t->testCall10(2, 3));
        $this->assertSame(13, $t->testCall11(6, 7));
        $this->assertSame(9, $t->testCall12(4, 5));

        $this->assertSame('hello parent public', $t->testCall13());
        $this->assertSame('hello parent protected', $t->testCall14());

        $this->assertSame('hello ScallParent', Scallparent::testCallStatic());
        $this->assertSame('hello Scall', $t::testCallStatic());
    }

    /**
     * @test
     * @link https://github.com/phalcon/zephir/issues/1622
     */
    public function shouldReturnInterpolatedMethodFromZephir()
    {
        $t = new Scall();
        $this->assertSame('hello Scall', $t->interpolatedStaticReturn());
    }

    /**
     * @test
     * @link https://github.com/phalcon/zephir/issues/1622
     */
    public function shouldEchoInterpolatedMethodFromZephir()
    {
        $t = new Scall();

        ob_start();
        $t->interpolatedStaticEcho();
        $content = ob_get_clean();

        $this->assertSame('hello Scall', $content);
    }
}
