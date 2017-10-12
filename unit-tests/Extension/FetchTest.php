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
        $this->assertSame(self::$test->testFetchPost(1), 'one');
        $this->assertSame(self::$test->testFetchPost('two'), 2);
        $this->assertSame(self::$test->testFetchPost('three'), array(3));
    }

    /**
     * @dataProvider fieldProvider
     */
    public function testValue($input, $expected, $has)
    {
        $this->assertSame(self::$test->hasValue($input), $has);
        $this->assertSame(self::$test->getValue($input), $expected);
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
