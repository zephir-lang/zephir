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

class McallChainedTest extends TestCase
{
    public function testChained1()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained1(), $t);
    }

    public function testChained2()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained2(), $t);
    }

    public function testChained3()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained3(), $t);
    }
}
