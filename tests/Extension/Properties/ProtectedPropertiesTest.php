<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\ProtectedProperties;

class ProtectedPropertiesTest extends TestCase
{
    /** @test */
    public function shouldGetDefaultValueOfProtectedPropertiesUsingGetters()
    {
        $test = new ProtectedProperties();

        $this->assertNull($test->getSomeNull());
        $this->assertNull($test->getSomeNullInitial());

        $this->assertFalse($test->getSomeFalse());
        $this->assertTrue($test->getSomeTrue());

        $this->assertEquals(10, $test->getSomeInteger());
        $this->assertEquals(10.25, $test->getSomeDouble());

        $this->assertEquals('test', $test->getSomeString());
    }

    /** @test */
    public function shouldSetAndGetProtectedPropertyUsingShortcuts()
    {
        $test = new ProtectedProperties();

        $rand = uniqid('', true);

        $test->setSomeVar($rand);
        $this->assertSame($rand, $test->getSomeVar());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2089
     */
    public function shouldSetAndGetProtectedPropertyUsingExtendedClass()
    {
        $test = new \ExtendsProtectedProperties();

        $this->assertEquals(['key' => 'value'], $test->getSomeArrayVar());

        $test->setSomeArrayVar(['key' => 'value']);
        $this->assertEquals(['key' => 'value'], $test->getSomeArrayVar());

        $test->setSomeArrayVar(['key2' => 'value2']);
        $this->assertEquals(['key2' => 'value2'], $test->getSomeArrayVar());
    }
}
