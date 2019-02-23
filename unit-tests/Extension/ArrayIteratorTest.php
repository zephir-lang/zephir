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

use PHPUnit\Framework\TestCase;

class ArrayIteratorTest extends TestCase
{
    public function testTest()
    {
        $itertest = new \Test\ArrayIteratorTest();

        $this->assertTrue(!empty($itertest->test()));
    }
}
