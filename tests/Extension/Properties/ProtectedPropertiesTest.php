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

class ExtendsProtectedProperties extends ProtectedProperties
{
    protected $someArrayVar = [
        'key' => 'value'
    ];
}

class ProtectedPropertiesTest extends TestCase
{
    public function testAssertations()
    {
        $test = new \Stub\Properties\ProtectedProperties();

        $this->assertNull($test->getSomeNull());
        $this->assertNull($test->getSomeNullInitial());
        $this->assertFalse($test->getSomeFalse());
        $this->assertTrue($test->getSomeTrue());
        $this->assertSame($test->getSomeInteger(), 10);
        $this->assertSame($test->getSomeDouble(), 10.25);
        $this->assertSame($test->getSomeString(), 'test');

        $test->setSomeVar(($rand = rand(1, 1000) * 100));
        $this->assertSame($test->getSomeVar(), $rand);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2089
     */
    public function test2089Issue()
    {
        $extend = new ExtendsProtectedProperties();
        $this->assertSame($extend->getSomeArrayVar(), ['key' => 'value']);

        $extend->setSomeArrayVar(['key' => 'value']);
        $this->assertSame($extend->getSomeArrayVar(), ['key' => 'value']);
    }
}
