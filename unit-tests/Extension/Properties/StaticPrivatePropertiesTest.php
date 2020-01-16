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
use Test\Properties\App;
use Test\Properties\StaticPrivateProperties;

class StaticPrivatePropertiesTest extends TestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1530
     */
    public function shouldGetInstanceByUsingSingleton()
    {
        $t = new App();

        $this->assertInstanceOf(StaticPrivateProperties::class, $t->getInstance());
        $this->assertInstanceOf(StaticPrivateProperties::class, StaticPrivateProperties::getInstance());
        $this->assertSame($t->getInstance(), StaticPrivateProperties::getInstance());
    }
}
