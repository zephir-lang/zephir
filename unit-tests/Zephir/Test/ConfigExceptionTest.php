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

namespace Zephir\Test;

use Zephir\ConfigException;

class ConfigExceptionTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test __contruct method.
     */
    public function testConstruct()
    {
        $configException = new ConfigException('foo', 25);
        $this->assertSame($configException->getCode(), 25);
    }
}
