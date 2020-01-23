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
use Test\McallDynamic;

class MCallDynamicTest extends TestCase
{
    /** @test */
    public function callDynamic()
    {
        $a = new McallDynamic();
        $this->assertSame(1, $a->method1());
        $this->assertSame(2, $a->testMagicCall1());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1751
     */
    public function callAnonymousFunctionWithContext()
    {
        $this->markTestSkipped(
        // FIXME
            'Does not work on macOs.'
        );

        $t = new McallDynamic();

        $this->assertSame('Caller:perform', $t->testCallAnonymousFunctionWithContext());
    }
}
