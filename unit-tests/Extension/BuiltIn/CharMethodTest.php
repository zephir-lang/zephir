<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\BuiltIn;

use PHPUnit\Framework\TestCase;
use Test\BuiltIn\CharMethods;

class CharMethodTest extends TestCase
{
    public function testModifications()
    {
        $t = new CharMethods();

        $this->assertSame('61', $t->getHex());
        $this->assertSame('68656C6C6F', $t->getHexForString('hello'));
    }
}
