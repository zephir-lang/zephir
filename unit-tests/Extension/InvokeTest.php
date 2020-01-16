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

class InvokeTest extends TestCase
{
    public function testIssue1522()
    {
        $x1 = \Test\Invoke::test();
        $x2 = \Test\Invoke::test();
        $this->assertSame('string', $x1);
        $this->assertSame('string', $x2);
    }
}
