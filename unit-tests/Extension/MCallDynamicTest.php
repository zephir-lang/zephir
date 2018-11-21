<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Test\McallDynamic;
use Zephir\Support\TestCase;

class MCallTestDynamic extends TestCase
{
    public function testCallDynamic()
    {
        $a = new McallDynamic;
        $this->assertSame($a->method1(), 1);
        $this->assertSame($a->testMagicCall1(), 2);
    }
}
