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
        $t = new PublicProperties();

        $this->assertNull($t->someNull);
        $this->assertNull($t->someNullInitial);
        $this->assertFalse($t->someFalse);
        $this->assertTrue($t->someTrue);
        $this->assertSame($t->someInteger, 10);
        $this->assertSame($t->someDouble, 10.25);
        $this->assertSame($t->someString, 'test');

        $this->assertInternalType('array', $t->someEmptyArray);
        $this->assertSame([1, 2, 3, 4], $t->someArray);
    }

    public function test394Issue()
    {
        $t = new PublicProperties();
        $this->assertTrue($t->test394Issue());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/537
     */
    public function test537Issue()
    {
        $t = new ExtendsPublicProperties();
        $this->assertSame($t->getSomeGetterSetterArray(), ['holy']);
    }
}
