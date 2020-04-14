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
use Test\Concat;

class ConcatTest extends TestCase
{
    /** @var Concat */
    private $test;

    public function setUp()
    {
        $this->test = new Concat();
    }

    /** @test */
    public function shouldConcatenateLocalVariableAndSelfVariable()
    {
        $this->assertNull($this->test->getTestProperty());

        $this->test->testConcatBySelfProperty('test');
        $this->assertSame('test', $this->test->getTestProperty());

        $this->test->testConcatBySelfProperty(' string');
        $this->assertSame('test string', $this->test->getTestProperty());
    }

    /** @test */
    public function shouldAppendVariableToString()
    {
        $this->assertSame('appendtest', $this->test->testConcat1());
    }

    /** @test */
    public function shouldConcatenateVariableWithString()
    {
        $this->assertSame('appendotherappendother', $this->test->testConcat2());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1573
     */
    public function shouldConcatenateStringsSimilarToIntegersNumbers()
    {
        $this->assertSame('21', $this->test->testConcat3());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function shouldConcatenateStringWithVarDouble()
    {
        $this->assertSame(
            'SELECT * FROM TEST WHERE value <= 946.5 AND value >= 473.25',
            $this->test->testConcat4(1893)
        );
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function shouldConcatenateStringWithDouble()
    {
        $this->assertSame(
            'Concatenated string with number 18.93000001',
            $this->test->testConcat5(18.93000001)
        );
    }
}
