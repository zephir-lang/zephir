<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use Zephir\Backends\ZendEngine2\Backend as Zend2;
use Zephir\Backends\ZendEngine3\Backend as Zend3;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Logger;
use Zephir\Parser;
use Zephir\Parser\Manager;
use Zephir\Support\TestCase;
use function Zephir\unlink_recursive;

class LifeCycleTest extends TestCase
{
    /**
     * Common directory.
     *
     * @var string
     */
    private $pwd;

    /**
     * Store the current directory before to be change.
     *
     * @return void
     */
    public function setUp()
    {
        $this->pwd = getcwd();
    }

    /**
     * Restore current directory, and clean config.json.
     *
     * @return void
     */
    public function tearDown()
    {
        if (getcwd() != $this->pwd) {
            if (file_exists('ext')) {
                unlink_recursive('ext/');
            }

            chdir($this->pwd);
        }
    }

    protected function createProject($backend)
    {
        chdir(ZEPHIRPATH . '/unit-tests/fixtures/lifecycle');

        try {
            $config = new Config();
            $logger = new Logger($config);
            $backend = new $backend($config);
            $parser = new Parser();
            $manager = new Manager($parser, $logger);
            $compiler = new Compiler($config, $logger, $backend, $manager);
            $compiler->createProjectFiles('lifecycle');
        } catch (\Exception $e) {
            $this->fail($e->getMessage());
        }
    }

    /** @test */
    public function shouldCreateProjectFilesUsingZendEngine2()
    {
        $this->createProject(Zend2::class);

        $this->assertSame(
            implode(PHP_EOL, file('expected2.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES))
        );
    }

    /** @test */
    public function shouldCreateProjectFilesUsingZendEngine3()
    {
        $this->createProject(Zend3::class);

        $this->assertSame(
            implode(PHP_EOL, file('expected3.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES))
        );
    }
}
