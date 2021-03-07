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
use Stub\ArraySearch;

final class ArraySearchTest extends TestCase
{
    private ArraySearch $test;

    protected function setUp(): void
    {
        $this->test = new ArraySearch();
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1609
     */
    public function testShouldSearchInTheExternalArrayForAGivenValueAndReturnTheFirstCorrespondingKey(): void
    {
        $needle = 'value';
        $haystack = ['.', '/', '0', '1', '2', '3', '4', 'value', 'OtherValue'];

        $this->assertEquals(7, $this->test->simpleSearch($needle, $haystack));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1609
     */
    public function testShouldSearchInTheInternalArrayForAGivenValueAndReturnTheFirstCorrespondingKey(): void
    {
        $this->assertEquals(7, $this->test->searchUsingArrayInsideZephir());
    }
}
