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
use Test\Properties\ExtendsPublicProperties;
use Test\Properties\PublicProperties;

class PublicPropertiesTest extends TestCase
{
    public function testAssertations()
    {
        $test = new PublicProperties();

        $this->assertNull($test->someNull);
        $this->assertNull($test->someNullInitial);
        $this->assertFalse($test->someFalse);
        $this->assertTrue($test->someTrue);
        $this->assertSame($test->someInteger, 10);
        $this->assertSame($test->someDouble, 10.25);
        $this->assertSame($test->someString, 'test');

        $this->assertInternalType('array', $test->someEmptyArray);
        $this->assertSame([1, 2, 3, 4], $test->someArray);
    }

    public function test394Issue()
    {
        $test = new PublicProperties();
        $this->assertTrue($test->test394Issue());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/537
     */
    public function test537Issue()
    {
        $test = new ExtendsPublicProperties();
        $this->assertSame($test->getSomeGetterSetterArray(), ['holy']);
    }
}
