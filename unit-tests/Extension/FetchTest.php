<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class FetchTest extends \PHPUnit_Framework_TestCase
{

    public static $test;

    public static function setUpBeforeClass()
    {
        self::$test = new \Test\FetchTest();

        $_POST = array(
            'somePOST' => 'some_post',
            'VALUEPOST' => 'some_post2',
            1 => 'one',
            'two' => 2,
            'three' => array(3),
        );

        self::$test->setValues(array(
            'someVALUE' => 'some_value',
            'VALUEPOST' => 'some_value2',
        ));
    }

    public function testPost()
    {
        $this->assertEquals(self::$test->testFetchPost(1), 'one');
        $this->assertEquals(self::$test->testFetchPost('two'), 2);
        $this->assertEquals(self::$test->testFetchPost('three'), array(3));
    }

    /**
     * @dataProvider fieldProvider
     */
    public function testValue($input, $expected, $has)
    {
        $this->assertEquals(self::$test->hasValue($input), $has);
        $this->assertEquals(self::$test->getValue($input), $expected);
    }

    public function fieldProvider()
    {
        /**
         * input, expected, exist
         */
        return array(
            array('somePOST', 'some_post', true),
            array('VALUEPOST', 'some_post2', true),
            array('someVALUE', 'some_value', true),
            array('someUNDEFINED', null, false),
            array(1, 'one', true),
            array('two', 2, true),
            array('three', array(3), true),
        );
    }

    public static function tearDownAfterClass()
    {
        self::$test = null;
    }
}
