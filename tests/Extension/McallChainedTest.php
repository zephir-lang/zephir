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
    /** @var \Stub\McallChained */
    private $test;

    public function setUp()
    {
        $this->test = new \Stub\McallChained();
    }

    public function testChained1()
    {
        $this->assertSame($this->test->testChained1(), $this->test);
    }

    public function testChained2()
    {
        $this->assertSame($this->test->testChained2(), $this->test);
    }

    public function testChained3()
    {
        $this->assertSame($this->test->testChained3(), $this->test);
    }
}
