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

namespace Extension\Integral;

use PHPUnit\Framework\TestCase;
use Stub\Sort;

final class SortTest extends TestCase
{
    public function testSort(): void
    {
        $data = $original = range(1, 50);
        shuffle($data);

        $test = new Sort();
        $this->assertSame($test->quick($data), $original);
    }
}
