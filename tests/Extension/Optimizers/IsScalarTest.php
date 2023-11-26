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

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\IsScalar;

final class IsScalarTest extends TestCase
{
    private IsScalar $test;

    protected function setUp(): void
    {
        $this->test = new IsScalar();
    }

    public function testVariable(): void
    {
        $this->assertTrue($this->test->testIntVar());
        $this->assertTrue($this->test->testDoubleVar());
        $this->assertTrue($this->test->testBoolVar());
        $this->assertTrue($this->test->testStringVar());
        $this->assertTrue($this->test->testVar());

        $this->assertFalse($this->test->testEmptyArrayVar());
    }

    public function testVariableParameter(): void
    {
        $this->assertTrue($this->test->testVarParameter(1));
        $this->assertTrue($this->test->testVarParameter(1.0));
        $this->assertTrue($this->test->testVarParameter(true));
        $this->assertTrue($this->test->testVarParameter(false));
        $this->assertTrue($this->test->testVarParameter(''));
        $this->assertTrue($this->test->testVarParameter('test string'));

        $this->assertFalse($this->test->testVarParameter([]));
        $this->assertFalse($this->test->testVarParameter([1, 2, 3, 4, 5]));
        $this->assertFalse($this->test->testVarParameter($this));
        $this->assertFalse(
            $this->test->testVarParameter(
                function (): void {
                }
            )
        );
        $this->assertFalse($this->test->testVarParameter(null));
    }
}
