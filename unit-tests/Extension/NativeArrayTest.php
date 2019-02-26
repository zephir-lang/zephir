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
use Test\NativeArray;

class NativeArrayTest extends TestCase
{
    public function testArray()
    {
        $t = new NativeArray();
        $this->assertSame([], $t->testArray1());
        $this->assertSame([1, 2, 3], $t->testArray2());
        $this->assertSame([1.1, 2.2, 3.3], $t->testArray3());
        $this->assertSame([false, true, false], $t->testArray4());
        $this->assertSame([null, null, null], $t->testArray5());
        $this->assertSame(['x', 'y', 'z'], $t->testArray6());
        $this->assertSame([1, 2, 3], $t->testArray7());
        $this->assertSame([1.0, 2.0, 3.0], $t->testArray8());
        $this->assertSame([true, false, true], $t->testArray9());
        $this->assertSame(['hello1',  'hello2',  'hello3'], $t->testArray10());
        $this->assertSame([[1, 2, 3], [4, 5, 6]], $t->testArray11());
        $this->assertSame([[[1, 2, 3]]], $t->testArray12());
        $this->assertSame([1 => 'hello1', 2 => 'hello2', 3 => 'hello3'], $t->testArray13());
        $this->assertSame(['hello1' => 1, 'hello2' => 2, 'hello3' => 3], $t->testArray14());
        $this->assertSame(['hello1' => true, 'hello2' => false, 'hello3' => true], $t->testArray15());
        $this->assertSame(['hello1' => 1.0, 'hello2' => 2.0, 'hello3' => 3.0], $t->testArray16());
        $this->assertSame(['hello1' => null, 'hello2' => null, 'hello3' => null], $t->testArray17());
        $this->assertSame(['hello1' => 'a', 'hello2' => 'b', 'hello3' => 'c'], $t->testArray18());
        $this->assertSame([0 => true, 1 => false, 2 => true], $t->testArray19());
        $this->assertSame([0 => 1.0, 1 => 2.0, 2 => 3.0], $t->testArray20());
        $this->assertSame([0 => null, 1 => null, 2 => null], $t->testArray21());
        $this->assertSame([0 => 4, 1 => 5, 2 => 6], $t->testArray22());
        $this->assertSame([0 => 0, 1 => 0, 2 => 0], $t->testArray23());
        $this->assertSame([0 => 0.0, 1 => 0.0, 2 => 0.0], $t->testArray24());
    }

    public function testArrayAccess()
    {
        $t = new NativeArray();
        $this->assertSame(1, $t->testArrayAccess1());
        $this->assertSame(1, $t->testArrayAccess2());
        $this->assertSame(1, $t->testArrayAccess3());
        $this->assertSame(1, $t->testArrayAccess4());
        $this->assertSame(1, $t->testArrayAccess5());
    }

    public function testArrayMultipleAccess()
    {
        $t = new NativeArray();
        $this->assertSame('a', $t->testArrayMultipleAccess1());
        $this->assertSame('b', $t->testArrayMultipleAccess2());
        $this->assertSame('b', $t->testArrayMultipleAccess3());
        $this->assertSame('b', $t->testArrayMultipleAccess4());
        $this->assertSame(0, $t->testArrayMultipleAccess5());
    }

    public function testArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertSame([4, 2, 3], $t->testArrayUpdate1());
        $this->assertSame([4, 2, 3], $t->testArrayUpdate2());
        $this->assertSame([4, 5, 3], $t->testArrayUpdate3());
        $this->assertSame(['a' => 4, 'b' => 2, 'c' => 3], $t->testArrayUpdate4());
    }

    public function testMultipleArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertSame(['x' => ['y' => null]], $t->testMultipleArrayUpdate1());
        $this->assertSame(['x' => ['y' => ['z' => null]]], $t->testMultipleArrayUpdate2());
        $this->assertSame([0 => [1 => null]], $t->testMultipleArrayUpdate3());
        $this->assertSame([0 => [1 => [2 => null]]], $t->testMultipleArrayUpdate4());
        $this->assertSame(['a' => [1 => ['c' => null]]], $t->testMultipleArrayUpdate5());
        $this->assertSame([0 => ['b' => [2 => null]]], $t->testMultipleArrayUpdate6());
        $this->assertSame(['a' => [1 => ['c' => true]], 0 => ['b' => [2 => false]]], $t->testMultipleArrayUpdate7());
        $this->assertSame(['a' => [0 => null, 1 => true, 2 => false]], $t->testMultipleArrayUpdate8());
        $this->assertSame(['a' => [0 => null, 1 => false]], $t->testMultipleArrayUpdate9());
        $this->assertSame(['a' => [0 => null, 1 => ['c' => false]]], $t->testMultipleArrayUpdate10());
        $this->assertSame(['y' => ['x' => null]], $t->testMultipleArrayUpdate11());
        $this->assertSame(['y' => ['x' => ['z' => null]]], $t->testMultipleArrayUpdate12());
    }

    public function testArrayKeys()
    {
        $t = new NativeArray();
        $this->assertSame(['test1', 'test2', 'test3'], $t->testArrayKeys(['test1' => 1, 'test2' => 2, 'test3' => 3]));
        $this->assertSame([0, 1, 2, 3, 4, 5], $t->testArrayKeys([1, 2, 3, 4, 5, 6]));
    }

    public function testImplodeArray()
    {
        $t = new NativeArray();
        $this->assertSame('test1|test2|test3', $t->testImplodeArray(['test1' => 1, 'test2' => 2, 'test3' => 3]));
    }

    public function testIssue110()
    {
        $t = new NativeArray();
        $this->assertSame('B|K|M|G|T|KB|MB|GB|TB', $t->issue110());
    }

    public function testIssue264()
    {
        $t = new NativeArray();
        $this->assertFalse($t->issue264([1, 2, 3]));
    }

    public function testIssue743()
    {
        $t = new NativeArray();
        $t->issue743a([42 => []]);

        $expected = [42 => ['str' => 'ok']];
        $this->assertSame($expected, $t->issue743a([42 => []]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => null]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => 42.7]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => 42]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => true]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => 'bad']]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => []]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => ['hey']]]));
        $this->assertSame($expected, $t->issue743a([42 => ['str' => new \stdClass()]]));

        $expected = ['str' => [42 => 'ok']];
        $this->assertSame($expected, $t->issue743b(['str' => []]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => null]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => 42.7]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => 42]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => true]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => 'bad']]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => []]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => ['hey']]]));
        $this->assertSame($expected, $t->issue743b(['str' => [42 => new \stdClass()]]));

        $expected = ['str' => ['hey' => 'ok']];
        $this->assertSame($expected, $t->issue743c(['str' => []]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => null]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => 42.7]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => 42]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => true]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => 'bad']]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => []]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => ['hey']]]));
        $this->assertSame($expected, $t->issue743c(['str' => ['hey' => new \stdClass()]]));
    }

    public function testIssue709()
    {
        $t = new NativeArray();
        $this->assertTrue($t->Issue709());
    }

    public function testIssue1140()
    {
        $t = new NativeArray();
        $this->assertSame(['phalcon' => '/var/www/html/phalcon'], $t->Issue1140('phalcon', '/var/www/html/phalcon'));
    }

    public function testIssue1159()
    {
        $t = new NativeArray();
        $this->assertSame([1], $t->Issue1159());
    }
}
