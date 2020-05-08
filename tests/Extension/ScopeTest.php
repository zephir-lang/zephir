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
use Test\Scope;

class ScopeTest extends TestCase
{
    public function testScope1()
    {
        $this->assertSame(Scope::test1(), 'testinternal_string');
        $this->assertSame(Scope::test2(), [15, '0_66_132_198_']);
        $this->assertSame(Scope::test3(), 'internal_0internal_1internal_2internal_3');
    }
}
