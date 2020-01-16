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

class EmptyTest extends TestCase
{
    public function testEmpty()
    {
        $t = new \Test\EmptyTest();

        $this->assertTrue($t->testDynamicVarArrayEmpty());
        $this->assertFalse($t->testDynamicVarArrayNotEmpty());
        $this->assertTrue($t->testEmptyString());
        $this->assertFalse($t->testNotEmptyString());
        $this->assertTrue($t->testString(''));
        $this->assertFalse($t->testString('this is a string'));
        $this->assertTrue($t->testString('0'));
    }
}
