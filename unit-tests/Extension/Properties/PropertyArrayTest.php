<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Test\Properties\PropertyArray;

class PropertyArrayTest extends TestCase
{
    public function testAssertations()
    {
        $test = new PropertyArray();
        $this->assertSame([1], $test->someArray);
        $test->appendSome('one');
        $this->assertSame([1, 'one'], $test->someArray);
    }

    public function testIssues1831()
    {
        $header = [
            'header 1',
            'header 2',
            'header 3',
        ];

        $data = [
            'header 1',
            'header 2',
            'header 3',
            'data 1',
            'data 2',
            'data 3',
            'data 4',
            'data 5',
            'data 6',
            'data 7',
            'data 8',
        ];

        $test = new PropertyArray();
        $test->setOtherArray($data);
        $this->assertSame($header, $test->testIssues1831());
    }
}
