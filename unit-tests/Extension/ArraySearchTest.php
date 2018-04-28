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

use Test\ArraySearch;

class ArraySearchTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1609
     */
    public function shouldSearchInTheExternalArrayForAGivenValueAndReturnTheFirstCorrespondingKey()
    {
        $t = new ArraySearch();

        $needle   = 'value';
        $haystack = [".", "/", "0", "1", "2", "3", "4", "value", "OtherValue"];

        $this->assertEquals(7, $t->simpleSearch($needle, $haystack));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1609
     */
    public function shouldSearchInTheInternalArrayForAGivenValueAndReturnTheFirstCorrespondingKey()
    {
        $t = new ArraySearch();
        $this->assertEquals(7, $t->searchUsingArrayInsideZephir());
    }
}
