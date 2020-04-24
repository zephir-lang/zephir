<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Integral;

use PHPUnit\Framework\TestCase;

class SortTest extends TestCase
{
    public function testSort()
    {
        $data = $original = range(1, 50);
        shuffle($data);

        $test = new \Test\Sort();
        $this->assertSame($test->quick($data), $original);
    }
}
