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

use Psr\Container\ContainerInterface;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\FileSystem\FileSystemInterface;
use function Zephir\unlink_recursive;

class LifeCycleTest extends KernelTestCase
{
    /**
     * Common directory.
     *
     * @var string
     */
    private $pwd;

    /**
     * Store the current directory before to be change.
     */
    public function setUp()
    {
        $this->pwd = getcwd();
    }

    /**
     * Restore current directory, and clean config.json.
     */
    public function tearDown()
    {
        if (getcwd() != $this->pwd) {
            $dotZephir = \dirname(\dirname(self::$kernel->getCacheDir()));
            if (file_exists($dotZephir)) {
                unlink_recursive($dotZephir);
            }

            if (file_exists(getcwd().'/ext')) {
                unlink_recursive(getcwd().'/ext');
            }

            chdir($this->pwd);
        }
    }

    /** @test */
    public function shouldCreateProjectFilesUsingZendEngine2()
    {
        $this->createProject('ZendEngine2');

        $this->assertSame(
            implode(PHP_EOL, file('expected2.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES))
        );
    }

    /** @test */
    public function shouldCreateProjectFilesUsingZendEngine3()
    {
        $this->createProject('ZendEngine3');

        $this->assertSame(
            implode(PHP_EOL, file('expected3.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/lifecycle.c', FILE_IGNORE_NEW_LINES))
        );
    }

    protected function createProject($backend)
    {
        chdir(\constant('ZEPHIRPATH').'/unit-tests/fixtures/lifecycle');
        putenv('ZEPHIR_BACKEND='.$backend);

        self::bootKernel();

        $container = self::$kernel->getContainer();

        /** @var \Zephir\FileSystem\HardDisk $compilerFs */
        $compilerFs = $container->get(FileSystemInterface::class);
        $compilerFs->setBasePath(self::$kernel->getCacheDir());

        $this->muteOutput($container);

        /** @var Compiler $compiler */
        $compiler = $container->get(Compiler::class);
        $compiler->createProjectFiles('lifecycle');
    }

    /**
     * @param ContainerInterface $container
     */
    protected function muteOutput(ContainerInterface $container)
    {
        $container->get(Config::class)->set('silent', true);
    }
}
