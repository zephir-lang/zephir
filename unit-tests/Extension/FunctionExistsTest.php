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
use Test\FunctionExists;

class FunctionExistsTest extends TestCase
{
    /** @var FunctionExists $test */
    private $test;

    public function setUp()
    {
        $this->test = new FunctionExists();
    }

    /**
     * @test
     * @dataProvider providerInternalFunctions
     * @issue https://github.com/phalcon/zephir/issues/1547
     *
     * @param string $func The internal (built-in) function name
     */
    public function shouldCorrectDetectestExistenceWithPassedName($func)
    {
        $this->assertTrue($this->test->testWithPassedName($func));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1547
     */
    public function shouldCorrectDetectestExistenceInsideTheZephirCode()
    {
        $expected = [
            'substr' => true,
            'cubstr' => false,
            'ucfirst' => true,
            'bcfirst' => false,
            'stripos' => true,
            'ktripos' => false,
            'trim' => true,
            'prim' => false,
        ];

        $this->assertSame($expected, $this->test->testBuiltInFunctions());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1547
     */
    public function shouldCorrectDetectestExistenceByUsingString()
    {
        $this->assertTrue($this->test->testWithString());
    }

    public function providerInternalFunctions()
    {
        $allFunctions = get_defined_functions();
        shuffle($allFunctions['internal']);

        $functions = array_map(
            function ($value) {
                return [$value];
            },
            $allFunctions['internal']
        );

        return \array_slice($functions, 0, 10);
    }
}
