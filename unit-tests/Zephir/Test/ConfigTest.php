<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Test;

use Zephir\Config;

class ConfigTest extends \PHPUnit_Framework_TestCase
{
    public function testGetWithoutNamespace()
    {
        $config = new Config();
        $config->set('verbose', false);
        $this->assertFalse($config->get('verbose'));
    }

    public function testGetWithNamespace()
    {
        $config = new Config();
        $config->get('unused-variable', true, 'warnings');
        $this->assertTrue($config->get('unused-variable', 'warnings'));
    }

    public function testSetWithoutNamespace()
    {
        $config = new Config();
        $config->set('config', true);
        $this->assertTrue($config->get('config'));
    }

    public function testSetWithNamespace()
    {
        $config = new Config();
        $config->set('unused-variable', false, 'warnings');
        $this->assertFalse($config->get('unused-variable', 'warnings'));
    }
}
