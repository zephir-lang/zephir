<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Integral;

use Zephir\Support\TestCase;

class FannkuchTest extends TestCase
{
    public function testFannkuch()
    {
        $t = new \Test\Fannkuch();
        $this->assertSame($t->process(5), [11, 5, 7]);
    }
}
