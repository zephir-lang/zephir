<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\ArrayObject;

final class ArrayObjectTest extends TestCase
{
    public function testSetGet(): void
    {
        $test = new ArrayObject();
        $this->assertInstanceOf('\ArrayObject', $test);

        if (version_compare(PHP_VERSION, '8.2.0', '<')) {
            $test->test_1 = 1;
            $this->assertSame(1, $test->test_1);
        }

        $test['test_2'] = 1;
        $this->assertSame(1, $test['test_2']);
    }
}
