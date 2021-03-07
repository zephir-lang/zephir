<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\InstanceOff;

class InstanceOffTest extends TestCase
{
    private InstanceOff $test;

    protected function setUp(): void
    {
        $this->test = new InstanceOff();
    }

    public function testInstanceOf1(): void
    {
        $this->assertTrue($this->test->testInstanceOf1());
    }

    public function testInstanceOf2(): void
    {
        $this->assertTrue($this->test->testInstanceOf2());
    }

    public function testInstanceOf3(): void
    {
        $this->assertFalse($this->test->testInstanceOf3());
    }

    public function testInstanceOf4(): void
    {
        $this->assertTrue($this->test->testInstanceOf4(new \ArrayIterator([])));
        $this->assertTrue($this->test->testInstanceOf4(new \ArrayObject()));

        $this->assertFalse($this->test->testInstanceOf4(1));
        $this->assertFalse($this->test->testInstanceOf4(1.25));
        $this->assertFalse($this->test->testInstanceOf4(true));
        $this->assertFalse($this->test->testInstanceOf4(false));
        $this->assertFalse($this->test->testInstanceOf4('test'));
        $this->assertFalse($this->test->testInstanceOf4([]));
        $this->assertFalse($this->test->testInstanceOf4(new \stdClass()));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/776
     * @issue https://github.com/phalcon/zephir/issues/822
     */
    public function testInstanceOf5(): void
    {
        $this->assertTrue($this->test->testInstanceOf5($this->test));
        $this->assertFalse($this->test->testInstanceOf5(new \stdClass()));
    }

    public function testInstanceOf6(): void
    {
        $this->assertTrue($this->test->testInstanceOf6());
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf7(): void
    {
        $this->assertTrue($this->test->testInstanceOf7($this->test));
        $this->assertFalse($this->test->testInstanceOf7(new \stdClass()));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf8(): void
    {
        $this->assertTrue($this->test->testInstanceOf8('\\Stub\\InstanceOff'));
        $this->assertFalse($this->test->testInstanceOf8('\\stdClass'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1828
     */
    public function testInstanceOf9(): void
    {
        $this->assertTrue($this->test->testInstanceOf9($this->test, '\\Stub\\InstanceOff'));
        $this->assertFalse($this->test->testInstanceOf9($this->test, '\\stdClass'));
    }
}
