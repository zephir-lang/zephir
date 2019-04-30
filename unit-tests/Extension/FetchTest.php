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

class FetchTest extends TestCase
{
    public static $test;

    public static function setUpBeforeClass()
    {
        self::$test = new \Test\FetchTest();

        $_POST = [
            'somePOST' => 'some_post',
            'VALUEPOST' => 'some_post2',
            1 => 'one',
            'two' => 2,
            'three' => [3],
        ];

        self::$test->setValues([
            'someVALUE' => 'some_value',
            'VALUEPOST' => 'some_value2',
        ]);
    }

    public static function tearDownAfterClass()
    {
        self::$test = null;
    }

    public function testPost()
    {
        $this->assertSame(self::$test->testFetchPost(1), 'one');
        $this->assertSame(self::$test->testFetchPost('two'), 2);
        $this->assertSame(self::$test->testFetchPost('three'), [3]);
    }

    /**
     * @dataProvider fieldProvider
     *
     * @param mixed $input
     * @param mixed $expected
     * @param mixed $has
     */
    public function testValue($input, $expected, $has)
    {
        $this->assertSame(self::$test->hasValue($input), $has);
        $this->assertSame(self::$test->getValue($input), $expected);
    }

    public function fieldProvider()
    {
        /*
         * input, expected, exist
         */
        return [
            ['somePOST', 'some_post', true],
            ['VALUEPOST', 'some_post2', true],
            ['someVALUE', 'some_value', true],
            ['someUNDEFINED', null, false],
            [1, 'one', true],
            ['two', 2, true],
            ['three', [3], true],
        ];
    }

    public function testFetch7()
    {
        $this->assertSame(self::$test->testFetchArray7(['params' => []], 'params'), []);
    }
}
