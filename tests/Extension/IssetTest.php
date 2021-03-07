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

final class IssetTest extends TestCase
{
    public string $b = 'a';
    private \Stub\IssetTest $test;

    /**
     * This property is used during test
     *
     * @var mixed
     */
    private $test2;

    protected function setUp(): void
    {
        $this->test = new \Stub\IssetTest();
    }

    public function testIssetArray(): void
    {
        $testArray = ['a', 'abc' => 'def', 'gef' => '123'];
        $this->assertTrue($this->test->testIssetArray1($testArray, 'abc'));
        $this->assertTrue(!$this->test->testIssetArray2($testArray, 12));
        $this->assertTrue($this->test->testIssetArray3($testArray, 'gef'));
        $this->assertTrue($this->test->testIssetArray4($testArray));
        $this->assertTrue(!$this->test->testIssetArray5($testArray));
    }

    public function testIssetProperties(): void
    {
        $this->assertTrue($this->test->testIssetProperty1($this));
        $this->assertTrue($this->test->testIssetProperty2($this, 'test2'));
        $this->assertFalse($this->test->testIssetProperty2($this, 'test3'));
        $this->assertTrue($this->test->testIssetProperty3($this));
    }

    public function testIssetDynamicProperty(): void
    {
        $this->assertTrue($this->test->testIssetDynamicProperty1());
        $this->assertTrue(!$this->test->testIssetDynamicProperty2($this));
        $this->s = ['a' => 'true'];
        $this->assertTrue($this->test->testIssetDynamicProperty2($this));
    }
}
