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
use Test\Scall;
use Test\Scallparent;

class ScallTest extends TestCase
{
    public function testScall()
    {
        $test = new Scall();

        $this->assertSame('hello public', $test->testMethod1());
        $this->assertSame('hello public', $test::testMethod1());
        $this->assertSame('hello public', Scall::testMethod1());

        $this->assertSame('hello public', $test->testCall1());
        $this->assertSame('hello protected', $test->testCall2());
        $this->assertSame('hello private', $test->testCall3());

        $this->assertSame(5, $test->testCall4(2, 3));
        $this->assertSame(13, $test->testCall5(6, 7));
        $this->assertSame(9, $test->testCall6(4, 5));

        $this->assertSame('hello public', $test->testCall7());
        $this->assertSame('hello protected', $test->testCall8());
        $this->assertSame('hello private', $test->testCall9());

        $this->assertSame(5, $test->testCall10(2, 3));
        $this->assertSame(13, $test->testCall11(6, 7));
        $this->assertSame(9, $test->testCall12(4, 5));

        $this->assertSame('hello parent public', $test->testCall13());
        $this->assertSame('hello parent protected', $test->testCall14());

        $this->assertSame('hello ScallParent', Scallparent::testCallStatic());
        $this->assertSame('hello Scall', $test::testCallStatic());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1622
     */
    public function shouldReturnInterpolatedMethodFromZephir()
    {
        $test = new Scall();
        $this->assertSame('hello Scall', $test->interpolatedStaticReturn());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1622
     */
    public function shouldEchoInterpolatedMethodFromZephir()
    {
        $test = new Scall();

        ob_start();
        $test->interpolatedStaticEcho();
        $content = ob_get_clean();

        $this->assertSame('hello Scall', $content);
    }
}
