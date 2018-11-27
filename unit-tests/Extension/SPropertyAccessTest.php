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
use Test\SPropertyAccess;

class SPropertyAccessTest extends TestCase
{
    public function testStaticPropertyAccess()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgument(','), ',');
        $this->assertSame($spa->testArgument(), '.');
    }

    public function testStaticPropertyAccessWithUnderscore()
    {
        $spa = new SPropertyAccess();
        $this->assertSame($spa->testArgumentWithUnderscore(','), ',');
        $this->assertSame($spa->testArgumentWithUnderscore(), '.');
    }
}
