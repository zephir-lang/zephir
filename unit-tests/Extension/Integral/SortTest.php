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

class SortTest extends TestCase
{
    public function testSort()
    {
        $data = $original = range(1, 50);
        shuffle($data);

        $t = new \Test\Sort();
        $this->assertSame($t->quick($data), $original);
    }
}
