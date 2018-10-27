<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Test\ArraySearch;
use Zephir\Support\TestCase;

class ArraySearchTest extends TestCase
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
