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
use Stub\Functions;

class FunctionsTest extends TestCase
{
    public function testFilterVar(): void
    {
        $class = new Functions();

        $this->assertFalse($class->filterVar1());
        $this->assertTrue($class->filterVar2());
    }
}
