<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\FunctionExists;

class FunctionExistsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @test
     * @dataProvider providerInternalFunctions
     * @issue 1547
     * @param string $func The internal (built-in) function name
     */
    public function shouldCorrectDetectestExistenceWithPassedName($func)
    {
        $t = new FunctionExists();
        $this->assertTrue($t->testWithPassedName($func));
    }

    /**
     * @test
     * @issue 1547
     */
    public function shouldCorrectDetectestExistenceInsideTheZephirCode()
    {
        $t = new FunctionExists();
        $expected = [
            "substr"  => true,
            "cubstr"  => false,
            "ucfirst" => true,
            "bcfirst" => false,
            "stripos" => true,
            "ktripos" => false,
            "trim"    => true,
            "prim"    => false,
        ];

        $this->assertSame($expected, $t->testBuiltInFunctions());
    }

    /**
     * @test
     * @issue 1547
     */
    public function shouldCorrectDetectestExistenceByUsingString()
    {
        $t = new FunctionExists();
        $this->assertTrue($t->testWithString());
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

        return array_slice($functions, 0, 10);
    }
}
