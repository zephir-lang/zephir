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
use Stub\Concat;

class ConcatTest extends TestCase
{
    /** @var Concat */
    private $test;

    protected function setUp(): void
    {
        $this->test = new Concat();
    }

    public function testShouldConcatenateLocalVariableAndSelfVariable()
    {
        $this->assertNull($this->test->getTestProperty());

        $this->test->testConcatBySelfProperty('test');
        $this->assertSame('test', $this->test->getTestProperty());

        $this->test->testConcatBySelfProperty(' string');
        $this->assertSame('test string', $this->test->getTestProperty());
    }

    public function testShouldAppendVariableToString()
    {
        $this->assertSame('appendtest', $this->test->testConcat1());
    }

    public function testShouldConcatenateVariableWithString()
    {
        $this->assertSame('appendotherappendother', $this->test->testConcat2());
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1573
     */
    public function testShouldConcatenateStringsSimilarToIntegersNumbers()
    {
        $this->assertSame('21', $this->test->testConcat3());
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function testShouldConcatenateStringWithVarDouble()
    {
        $this->assertSame(
            'SELECT * FROM TEST WHERE value <= 946.5 AND value >= 473.25',
            $this->test->testConcat4(1893)
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function testShouldConcatenateStringWithDouble()
    {
        $this->assertSame(
            'Concatenated string with number 18.93000001',
            $this->test->testConcat5(18.93000001)
        );
    }
}
