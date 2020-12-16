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
use Stub\McallDynamic;

class MCallDynamicTest extends TestCase
{
    public function testCallDynamic()
    {
        $test = new McallDynamic();
        $this->assertSame(1, $test->method1());
        $this->assertSame(2, $test->testMagicCall1());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1751
     */
    public function testCallAnonymousFunctionWithContext()
    {
        $test = new McallDynamic();

        $this->assertSame('Caller:perform', $test->testCallAnonymousFunctionWithContext());
    }
}
