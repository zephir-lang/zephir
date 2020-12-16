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
use Stub\FunctionExists;

class FunctionExistsTest extends TestCase
{
    /** @var FunctionExists */
    private $test;

    protected function setUp(): void
    {
        $this->test = new FunctionExists();
    }

    /**
     * @dataProvider providerInternalFunctions
     * @issue https://github.com/phalcon/zephir/issues/1547
     *
     * @param string $func The internal (built-in) function name
     */
    public function testShouldCorrectDetectestExistenceWithPassedName($func)
    {
        $this->assertTrue($this->test->testWithPassedName($func));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1547
     */
    public function testShouldCorrectDetectestExistenceInsideTheZephirCode()
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
     * @issue https://github.com/phalcon/zephir/issues/1547
     */
    public function testShouldCorrectDetectestExistenceByUsingString()
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
