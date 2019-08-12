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
use Test\Concat;

class ConcatTest extends TestCase
{
    /** @test */
    public function shouldConcatenateLocalVariableAndSelfVariable()
    {
        $t = new Concat();

        $this->assertNull($t->getTestProperty());

        $t->testConcatBySelfProperty('test');
        $this->assertSame('test', $t->getTestProperty());

        $t->testConcatBySelfProperty(' string');
        $this->assertSame('test string', $t->getTestProperty());
    }

    /** @test */
    public function shouldAppendVariableToString()
    {
        $t = new Concat();
        $this->assertSame('appendtest', $t->testConcat1());
    }

    /** @test */
    public function shouldConcatenateVariableWithString()
    {
        $t = new Concat();
        $this->assertSame('appendotherappendother', $t->testConcat2());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1573
     */
    public function shouldConcatenateStringsSimilarToIntegersNumbers()
    {
        $t = new Concat();
        $this->assertSame('21', $t->testConcat3());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function shouldConcatenateStringWithVarDouble()
    {
        $t = new Concat();
        $this->assertSame(
            /* @lang text */ 'SELECT * FROM TEST WHERE value <= 946.5 AND value >= 473.25',
            $t->testConcat4(1893)
        );
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1893
     */
    public function shouldConcatenateStringWithDouble()
    {
        $t = new Concat();
        $this->assertSame(
            'Concatenated string with number 18.93000001',
            $t->testConcat5(18.93000001)
        );
    }
}
