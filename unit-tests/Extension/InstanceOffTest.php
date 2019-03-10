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
use Test\InstanceOff;

class InstanceOffTest extends TestCase
{
    public function testInstanceOf1()
    {
        $t = new InstanceOff();
        $this->assertTrue($t->testInstanceOf1());
    }

    public function testInstanceOf2()
    {
        $t = new InstanceOff();
        $this->assertTrue($t->testInstanceOf2());
    }

    public function testInstanceOf3()
    {
        $t = new InstanceOff();
        $this->assertFalse($t->testInstanceOf3());
    }

    public function testInstanceOf4()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf4(new \ArrayIterator([])));
        $this->assertTrue($t->testInstanceOf4(new \ArrayObject()));

        $this->assertFalse($t->testInstanceOf4(1));
        $this->assertFalse($t->testInstanceOf4(1.25));
        $this->assertFalse($t->testInstanceOf4(true));
        $this->assertFalse($t->testInstanceOf4(false));
        $this->assertFalse($t->testInstanceOf4('test'));
        $this->assertFalse($t->testInstanceOf4([]));
        $this->assertFalse($t->testInstanceOf4(new \stdClass()));
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/776
     */
    public function testInstanceOf5()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf5($t));

        /*
         * Cannot test instance of for not objects
         * @link https://github.com/phalcon/zephir/issues/822
         */

        $this->assertFalse($t->testInstanceOf5(new \stdClass()));
    }

    public function testInstanceOf6()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf6());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf7()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf7($t));
        $this->assertFalse($t->testInstanceOf7(new \stdClass()));
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf8()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf8('\\Test\\InstanceOff'));
        $this->assertFalse($t->testInstanceOf8('\\stdClass'));
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf9()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf9($t, '\\Test\\InstanceOff'));
        $this->assertFalse($t->testInstanceOf9($t, '\\stdClass'));
    }
}
