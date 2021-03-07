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
use Stub\Fannkuch;

final class FannkuchTest extends TestCase
{
    public function testFannkuch(): void
    {
        $test = new Fannkuch();
        $this->assertSame($test->process(5), [11, 5, 7]);
    }
}
