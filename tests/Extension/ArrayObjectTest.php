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
use Stub\ArrayObject;

class ArrayObjectTest extends TestCase
{
    public function testSetGet()
    {
        $test = new ArrayObject();
        $this->assertInstanceOf('\ArrayObject', $test);

        $test->test_1 = 1;
        $this->assertSame(1, $test->test_1);

        $test['test_2'] = 1;
        $this->assertSame(1, $test['test_2']);
    }
}
