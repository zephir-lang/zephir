<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\ArrayObject;

final class ArrayObjectTest extends TestCase
{
    public function testSetGet(): void
    {
        $test = new ArrayObject();
        $this->assertInstanceOf('\ArrayObject', $test);

        $test->test_1 = 1;
        $this->assertSame(1, $test->test_1);

        $test['test_2'] = 1;
        $this->assertSame(1, $test['test_2']);
    }
}
