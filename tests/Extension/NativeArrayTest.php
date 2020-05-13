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
use Stub\NativeArray;

class NativeArrayTest extends TestCase
{
    /** @var NativeArray */
    private $test;

    public function setUp()
    {
        $this->test = new NativeArray();
    }

    public function testArray()
    {
        $this->assertSame([], $this->test->testArray1());
        $this->assertSame([1, 2, 3], $this->test->testArray2());
        $this->assertSame([1.1, 2.2, 3.3], $this->test->testArray3());
        $this->assertSame([false, true, false], $this->test->testArray4());
        $this->assertSame([null, null, null], $this->test->testArray5());
        $this->assertSame(['x', 'y', 'z'], $this->test->testArray6());
        $this->assertSame([1, 2, 3], $this->test->testArray7());
        $this->assertSame([1.0, 2.0, 3.0], $this->test->testArray8());
        $this->assertSame([true, false, true], $this->test->testArray9());
        $this->assertSame(['hello1',  'hello2',  'hello3'], $this->test->testArray10());
        $this->assertSame([[1, 2, 3], [4, 5, 6]], $this->test->testArray11());
        $this->assertSame([[[1, 2, 3]]], $this->test->testArray12());
        $this->assertSame([1 => 'hello1', 2 => 'hello2', 3 => 'hello3'], $this->test->testArray13());
        $this->assertSame(['hello1' => 1, 'hello2' => 2, 'hello3' => 3], $this->test->testArray14());
        $this->assertSame(['hello1' => true, 'hello2' => false, 'hello3' => true], $this->test->testArray15());
        $this->assertSame(['hello1' => 1.0, 'hello2' => 2.0, 'hello3' => 3.0], $this->test->testArray16());
        $this->assertSame(['hello1' => null, 'hello2' => null, 'hello3' => null], $this->test->testArray17());
        $this->assertSame(['hello1' => 'a', 'hello2' => 'b', 'hello3' => 'c'], $this->test->testArray18());
        $this->assertSame([0 => true, 1 => false, 2 => true], $this->test->testArray19());
        $this->assertSame([0 => 1.0, 1 => 2.0, 2 => 3.0], $this->test->testArray20());
        $this->assertSame([0 => null, 1 => null, 2 => null], $this->test->testArray21());
        $this->assertSame([0 => 4, 1 => 5, 2 => 6], $this->test->testArray22());
        $this->assertSame([0 => 0, 1 => 0, 2 => 0], $this->test->testArray23());
        $this->assertSame([0 => 0.0, 1 => 0.0, 2 => 0.0], $this->test->testArray24());
    }

    public function testArrayAccess()
    {
        $this->assertSame(1, $this->test->testArrayAccess1());
        $this->assertSame(1, $this->test->testArrayAccess2());
        $this->assertSame(1, $this->test->testArrayAccess3());
        $this->assertSame(1, $this->test->testArrayAccess4());
        $this->assertSame(1, $this->test->testArrayAccess5());
    }

    public function testArrayMultipleAccess()
    {
        $this->assertSame('a', $this->test->testArrayMultipleAccess1());
        $this->assertSame('b', $this->test->testArrayMultipleAccess2());
        $this->assertSame('b', $this->test->testArrayMultipleAccess3());
        $this->assertSame('b', $this->test->testArrayMultipleAccess4());
        $this->assertSame(0, $this->test->testArrayMultipleAccess5());
    }

    public function testArrayUpdate()
    {
        $this->assertSame([4, 2, 3], $this->test->testArrayUpdate1());
        $this->assertSame([4, 2, 3], $this->test->testArrayUpdate2());
        $this->assertSame([4, 5, 3], $this->test->testArrayUpdate3());
        $this->assertSame(['a' => 4, 'b' => 2, 'c' => 3], $this->test->testArrayUpdate4());
    }

    public function testMultipleArrayUpdate()
    {
        $this->assertSame(['x' => ['y' => null]], $this->test->testMultipleArrayUpdate1());
        $this->assertSame(['x' => ['y' => ['z' => null]]], $this->test->testMultipleArrayUpdate2());
        $this->assertSame([0 => [1 => null]], $this->test->testMultipleArrayUpdate3());
        $this->assertSame([0 => [1 => [2 => null]]], $this->test->testMultipleArrayUpdate4());
        $this->assertSame(['a' => [1 => ['c' => null]]], $this->test->testMultipleArrayUpdate5());
        $this->assertSame([0 => ['b' => [2 => null]]], $this->test->testMultipleArrayUpdate6());
        $this->assertSame(['a' => [1 => ['c' => true]], 0 => ['b' => [2 => false]]], $this->test->testMultipleArrayUpdate7());
        $this->assertSame(['a' => [0 => null, 1 => true, 2 => false]], $this->test->testMultipleArrayUpdate8());
        $this->assertSame(['a' => [0 => null, 1 => false]], $this->test->testMultipleArrayUpdate9());
        $this->assertSame(['a' => [0 => null, 1 => ['c' => false]]], $this->test->testMultipleArrayUpdate10());
        $this->assertSame(['y' => ['x' => null]], $this->test->testMultipleArrayUpdate11());
        $this->assertSame(['y' => ['x' => ['z' => null]]], $this->test->testMultipleArrayUpdate12());
    }

    public function testArrayKeys()
    {
        $this->assertSame(['test1', 'test2', 'test3'], $this->test->testArrayKeys(['test1' => 1, 'test2' => 2, 'test3' => 3]));
        $this->assertSame([0, 1, 2, 3, 4, 5], $this->test->testArrayKeys([1, 2, 3, 4, 5, 6]));
    }

    public function testImplodeArray()
    {
        $this->assertSame('test1|test2|test3', $this->test->testImplodeArray(['test1' => 1, 'test2' => 2, 'test3' => 3]));
    }

    public function testIssue110()
    {
        $this->assertSame('B|K|M|G|T|KB|MB|GB|TB', $this->test->issue110());
    }

    public function testIssue264()
    {
        $this->assertFalse($this->test->issue264([1, 2, 3]));
    }

    public function testIssue743()
    {
        $this->test->issue743a([42 => []]);

        $expected = [42 => ['str' => 'ok']];
        $this->assertSame($expected, $this->test->issue743a([42 => []]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => null]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => 42.7]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => 42]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => true]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => 'bad']]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => []]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => ['hey']]]));
        $this->assertSame($expected, $this->test->issue743a([42 => ['str' => new \stdClass()]]));

        $expected = ['str' => [42 => 'ok']];
        $this->assertSame($expected, $this->test->issue743b(['str' => []]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => null]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => 42.7]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => 42]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => true]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => 'bad']]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => []]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => ['hey']]]));
        $this->assertSame($expected, $this->test->issue743b(['str' => [42 => new \stdClass()]]));

        $expected = ['str' => ['hey' => 'ok']];
        $this->assertSame($expected, $this->test->issue743c(['str' => []]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => null]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => 42.7]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => 42]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => true]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => 'bad']]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => []]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => ['hey']]]));
        $this->assertSame($expected, $this->test->issue743c(['str' => ['hey' => new \stdClass()]]));
    }

    public function testIssue709()
    {
        $this->assertTrue($this->test->Issue709());
    }

    public function testIssue1140()
    {
        $this->assertSame(['phalcon' => ['/var/www/html/phalcon']], $this->test->Issue1140('phalcon', '/var/www/html/phalcon'));
    }

    public function testIssue1159()
    {
        $this->assertSame([1], $this->test->Issue1159());
    }
}
