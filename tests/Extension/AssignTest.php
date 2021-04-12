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
use Stub\Assign;

final class AssignTest extends TestCase
{
    private Assign $test;

    protected function setUp(): void
    {
        $this->test = new Assign();
    }

    /**
     * @dataProvider variableAssignProvider
     *
     * @param mixed  $expected
     * @param string $test
     * @param mixed  $testParams
     */
    public function testShouldPerformAssignment($expected, $test, $testParams = null): void
    {
        $this->assertSame($expected, \call_user_func([$this->test, $test], $testParams));
    }

    public function variableAssignProvider(): array
    {
        $arry = [
            'a' => [
                'b_key' => 'b_val',
                'b' => ['d_key' => 'd_val', 'c' => ['d' => ['e' => 'f']]],
            ],
            's' => 1,
            1 => [2 => [3 => 4, 5 => 6, 'abc' => 'abc']],
        ];

        return [
            [1,              'testAssign1'],
            [1,              'testAssign2'],
            [0,              'testAssign3'],
            [0,              'testAssign4'],
            [2,              'testAssign5'],
            [true,           'testAssign6'],
            [true,           'testAssign7'],
            [true,           'testAssign8'],
            [false,          'testAssign9'],
            [false,          'testAssign10'],
            [0.0,            'testAssign11'],
            [4.0,            'testAssign12'],
            [0.0,            'testAssign13'],
            [1.0,            'testAssign14'],
            [5.0,            'testAssign15'],
            [1,              'testAssign16'],
            [1.0,            'testAssign17'],
            [false,          'testAssign18'],
            [true,           'testAssign19'],
            [null,           'testAssign20'],
            [1,              'testAssign21'],
            [1.0,            'testAssign22'],
            [true,           'testAssign23'],
            [1.0,            'testAssign24'],
            [1.0,            'testAssign25'],
            [1,              'testAssign26'],
            [1.0,            'testAssign27'],
            [true,           'testAssign28'],
            [true,           'testAssign29'],
            [0,              'testAssign30'],
            [0.0,            'testAssign31'],
            [false,          'testAssign32'],
            [false,          'testAssign33'],
            [false,          'testAssign34'],
            [false,          'testAssign35'],
            [false,          'testAssign36'],
            [$arry,          'testAssign37'],
            [['i' => 'val'], 'testAssign38', 'i'],
            [150,            'testAssign39'],
            [116,            'testAssign40'],
            [21,             'testAssign41', 2],
            [43,             'testAssign42', 86],
            [1,              'testAssign43', 2],
            [1,              'testAssign44', 41],
        ];
    }

    /**
     * @dataProvider propertyAssignProvider
     *
     * @param mixed  $expected
     * @param string $test
     * @param mixed  $testParams
     */
    public function testShouldPerformAssignmentForProperties($expected, $test, $testParams = null): void
    {
        if ('array' === \gettype($testParams)) {
            $this->assertSame($expected, \call_user_func_array([$this->test, $test], $testParams));
        } else {
            $this->assertSame($expected, \call_user_func([$this->test, $test], $testParams));
        }
    }

    public function propertyAssignProvider(): array
    {
        $arry = [
            'a' => [
                'b_key' => 'b_val',
                'b' => ['d_key' => 'd_val', 'c' => ['d' => ['e' => 'f']]],
            ],
            1 => [2 => [3 => 4, 5 => 6, 'abc' => 'abc']],
            's' => 1,
        ];

        return [
            [2,                           'testPropertyIncr1'],
            [2,                           'testPropertyAddAssign1'],
            [3,                           'testPropertyAddAssign2'],
            [2,                           'testPropertyAssignValuePlus1'],
            [1,                           'testPropertyDecr'],
            [-2,                          'testPropertySubAssign1'],
            [-1,                          'testPropertySubAssign2'],
            [2,                           'testPropertyMulAssign1'],
            [3,                           'testPropertyMulAssign2'],
            ['test string',               'testPropertyAssignStringConcat'],
            [['test_index' => 'value'],   'testArrayVarAssign1', ['test_index', 'value']],
            [['test_index' => 'value'],   'testArrayVarAssign2', ['test_index', 'value']],
            [$arry,                       'testPropertyArray14'],
            [$arry,                       'testStaticPropertyArrayMulti4'],
            [['test', 1, 1.5, false, []], 'testStaticPropertyArrayAppend'],
            [['a' => true, 'b' => false], 'testArrayBoolExpressionAssign'],
        ];
    }

    public function testGlobalVarAssign(): void
    {
        /*
         * @see LetStatement::_assignArrayIndexSingle();
         */
        $this->assertTrue(!isset($_POST['test_index']));
        $this->test->testGlobalVarAssign('test_index', 'value');
        $this->assertSame($_POST['test_index'], 'value');

        /* Check primitive types */
        $this->test->testAssignSuperGlobals();
        $this->assertSame($_GET['steststr'], 'stest');
        $this->assertSame($_GET['steststr2'], 'stest2');
        $this->assertSame($_GET['stestint'], 1);
        $this->assertSame($_GET['stestint2'], 2);
        $this->assertSame($_GET['stest2'], 'testval');
    }

    public function testGlobalVarAssignSERVER(): void
    {
        $serverCount = \count($_SERVER);
        $this->test->testAssignSuperGlobalsSERVER();

        $this->assertCount($serverCount + 2, $_SERVER);
    }

    public function testGlobalVarAssignGET(): void
    {
        $getCount = \count($_GET);
        $this->test->testAssignSuperGlobalsGET();

        $this->assertCount($getCount + 2, $_GET);
    }

    public function testIssue597()
    {
        $_GET = [];
        $this->test->issue597();

        $this->assertEmpty($_GET);

        $_GET['s'] = 10;
        $this->test->issue597();

        $this->assertSame(50, $_GET['s']);
    }
}
