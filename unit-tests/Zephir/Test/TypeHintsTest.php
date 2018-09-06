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
use Zephir\Commands\CommandGenerate;
use Zephir\Commands\Manager as CommandsManager;
use Zephir\Parser\Manager as ParserManager;
use Zephir\Backends\ZendEngine2\Backend as Zend2;
use Zephir\Backends\ZendEngine3\Backend as Zend3;

class TypeHintsTest extends \PHPUnit_Framework_TestCase
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

    public function testTypeHintsZend2()
    {
        chdir(__DIR__ . DIRECTORY_SEPARATOR . '_files/typehints');
        $config = new Config();
        $logger = new Logger($config);
        $backend = new Zend2($config);
        $parser = new ParserManager(new Parser(), $logger);
        $compiler = new Compiler($config, $logger, $backend, $parser);
        $command = new CommandGenerate(new CommandsManager);
        $compiler->generate($command);
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected2.c', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/args.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_args2.h', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/retval.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_retval2.h', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/both.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_both2.h', FILE_IGNORE_NEW_LINES))
        );
    }

    public function testTypeHintsZend3()
    {
        chdir(__DIR__ . DIRECTORY_SEPARATOR . '_files/typehints');
        $config = new Config();
        $logger = new Logger($config);
        $backend = new Zend3($config);
        $parser = new ParserManager(new Parser(), $logger);
        $compiler = new Compiler($config, $logger, $backend, $parser);
        $command = new CommandGenerate(new CommandsManager);
        $compiler->generate($command);
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected3.c', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/args.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_args3.h', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/retval.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_retval3.h', FILE_IGNORE_NEW_LINES))
        );
        $this->assertSame(
            implode(PHP_EOL, file('ext/typehints/both.zep.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('expected_both3.h', FILE_IGNORE_NEW_LINES))
        );
    }

    /**
     * Restore current directory, and clean config.json.
     */
    public function tearDown()
    {
        if (file_exists('.temp')) {
            unlink_recursive('.temp/');
        }

        if (file_exists('ext')) {
            unlink_recursive('ext/');
        }

        if (getcwd() != $this->pwd) {
            chdir($this->pwd);
        }
    }
}
