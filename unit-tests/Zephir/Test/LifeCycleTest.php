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
use Zephir\Compiler;
use Zephir\Logger;
use Zephir\Parser;
use Zephir\Parser\Manager;
use Zephir\Backends\ZendEngine2\Backend as Zend2;
use Zephir\Backends\ZendEngine3\Backend as Zend3;

class LifeCycleTest extends \PHPUnit_Framework_TestCase
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

    public function testLifeCycleInjectionZend2()
    {
        chdir(__DIR__ . DIRECTORY_SEPARATOR . '_files/lifecycle');
        $config = new Config();
        $logger = new Logger($config);
        $backend = new Zend2($config);
        $parser = new Parser();
        $manager = new Manager($parser, $logger);
        $compiler = new Compiler($config, $logger, $backend, $manager);
        $compiler->createProjectFiles('lifecycle');
        $this->assertSame(
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected2.c', FILE_IGNORE_NEW_LINES))
        );
    }

    public function testLifeCycleInjectionZend3()
    {
        chdir(__DIR__ . DIRECTORY_SEPARATOR . '_files/lifecycle');
        $config = new Config();
        $logger = new Logger($config);
        $backend = new Zend3($config);
        $parser = new Parser();
        $manager = new Manager($parser, $logger);
        $compiler = new Compiler($config, $logger, $backend, $manager);
        $compiler->createProjectFiles('lifecycle');
        $this->assertSame(
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected3.c', FILE_IGNORE_NEW_LINES))
        );
    }

    /**
     * Restore current directory, and clean config.json.
     */
    public function tearDown()
    {
        if (file_exists('ext')) {
            exec('rm -rf ext/');
        }

        if (getcwd() != $this->pwd) {
            chdir($this->pwd);
        }
    }
}
