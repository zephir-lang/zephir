<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Config;

class ConfigTest extends TestCase
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
     * Test when we have a bad config.json file.
     *
     * @expectedException \Zephir\Exception
     * @expectedExceptionMessage The config.json file is not valid or there is
     * no Zephir extension initialized in this directory.
     */
    public function testConstructWithBadConfigFile()
    {
        chdir(\constant('ZEPHIRPATH').'/unit-tests/fixtures/badconfig');
        new Config();
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
     * Clean config.json file into tmp dir.
     */
    private function cleanTmpConfigFile()
    {
        /* clean config.json into tmp dir */
        $tmpConfigFile = sys_get_temp_dir().\DIRECTORY_SEPARATOR.'config.json';

        if (file_exists($tmpConfigFile)) {
            unlink($tmpConfigFile);
        }
    }

    /** @test */
    public function directiveShowsWithoutDuplicatedNamespace()
    {
        $config = new Config();
        $config->set('ext.some_key', 'some_value', 'test');

        $this->assertSame($config->get('ext.some_key', 'test'), 'some_value');
        $this->assertSame($config->get('test.my_setting_1', 'test'), null);
    }
}
