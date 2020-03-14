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
        $test = new \Test\EmptyTest();

        $this->assertTrue($test->testDynamicVarArrayEmpty());
        $this->assertFalse($test->testDynamicVarArrayNotEmpty());
        $this->assertTrue($test->testEmptyString());
        $this->assertFalse($test->testNotEmptyString());
        $this->assertTrue($test->testString(''));
        $this->assertFalse($test->testString('this is a string'));
        $this->assertTrue($test->testString('0'));
    }
}
