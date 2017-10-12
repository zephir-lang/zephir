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

use Zephir\Config;

class ConfigTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Common directory.
     *
     * @var string
     */
    private $pwd;

    public function setUp()
    {
        /* Store the current directory before to be change */
        $this->pwd = getcwd();
    }

    /**
     * Test when we have a bad config.json file.
     *
     * @expectedException Exception
     * @expectedExceptionMessage The config.json file is not valid or there is
     * no Zephir extension initialized in this directory.
     */
    public function testConstructWithBadConfigFile()
    {
        chdir(__DIR__ . DIRECTORY_SEPARATOR . '_files');
        $config = new Config();
    }

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

    /**
     * Test saveOnExit method.
     */
    public function testSaveOnExit()
    {
        chdir(sys_get_temp_dir());
        $config = new Config();
        $config->set('name', 'foo');
        $config->dumpToFile();
        $configJson = json_decode(file_get_contents('config.json'), true);
        $this->assertInternalType('array', $configJson);
        $this->assertSame($configJson['name'], 'foo');
        $this->cleanTmpConfigFile();
    }

    /**
     * Restore current directory, and clean config.json.
     */
    public function tearDown()
    {
        if (getcwd() != $this->pwd) {
            chdir($this->pwd);
        }

        $this->cleanTmpConfigFile();
    }

    /**
     * Clean config.json file into tmp dir.
     */
    private function cleanTmpConfigFile()
    {
        /* clean config.json into tmp dir */
        $tmpConfigFile = sys_get_temp_dir() . DIRECTORY_SEPARATOR . 'config.json';

        if (file_exists($tmpConfigFile)) {
            unlink($tmpConfigFile);
        }
    }
}
