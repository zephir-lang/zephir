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
use Stub\Fibonnaci;

final class FibonnaciTest extends TestCase
{
    public function testMethods(): void
    {
        $test = new Fibonnaci();

        $this->assertSame($test->fibInt(), 13);
        $this->assertSame($test->fibDouble(), 13.0);
        $this->assertSame($test->fibArray(), [
            0, 1, 1, 2, 3, 5, 8, 13, 21, 34,
        ]);
        $this->assertSame($test->fibArray2(), [
            0, 1, 1, 2, 3, 5, 8, 13, 21, 34,
        ]);
    }
}
