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

class ClosureTest extends TestCase
{
    public function testUseCommand()
    {
        $t = new \Test\Closures();

        $this->assertSame(2, $t->testUseCommand()());
    }
}
