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

class ClosureTest extends TestCase
{
    public function testUseCommand()
    {
        $test = new \Stub\Closures();

        $this->assertSame(2, $test->testUseCommand()());
    }
}
