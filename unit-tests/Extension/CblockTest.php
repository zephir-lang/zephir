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

use Zephir\Support\TestCase;

class CblockTest extends TestCase
{
    public function testBlock()
    {
        $t = new \Test\Cblock();
        $this->assertSame($t->testCblock1(), 10);
        $this->assertSame($t->testCblock2(), 55);
    }
}
