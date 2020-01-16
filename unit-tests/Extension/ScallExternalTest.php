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

class ScallExternalTest extends TestCase
{
    public function testCall1()
    {
        $t = new \Test\ScallExternal();
        $this->assertSame($t->testCall1(), 'hello public');
    }

    public function testCall2()
    {
        $t = new \Test\ScallExternal();
        $this->assertSame($t->testCall2(2, 3), 5);
    }
}
