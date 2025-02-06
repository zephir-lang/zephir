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
use Stub\Scall;
use Stub\Scallparent;

final class ScallTest extends TestCase
{
    public function testScall(): void
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

        if (version_compare(PHP_VERSION, '8.2.0', '<')) {
            $this->assertSame('hello ScallParent', Scallparent::testCallStatic());
            $this->assertSame('hello Scall', $test::testCallStatic());
        }
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1622
     */
    public function testShouldReturnInterpolatedMethodFromZephir(): void
    {
        $test = new Scall();
        $this->assertSame('hello Scall', $test->interpolatedStaticReturn());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1622
     */
    public function testShouldEchoInterpolatedMethodFromZephir(): void
    {
        $test = new Scall();

        ob_start();
        $test->interpolatedStaticEcho();
        $content = ob_get_clean();

        $this->assertSame('hello Scall', $content);
    }
}
