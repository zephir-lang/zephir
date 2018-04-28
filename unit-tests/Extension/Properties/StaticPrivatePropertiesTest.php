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

namespace Extension\Properties;

use Test\Properties\App;
use Test\Properties\StaticPrivateProperties;

class StaticPrivatePropertiesTest extends \PHPUnit_Framework_TestCase
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
