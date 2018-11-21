<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Command;

use League\Container\Container;
use League\Flysystem\Adapter\Local;
use League\Flysystem\Filesystem;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Tester\CommandTester;
use Zephir\Application;
use Zephir\Command\CompileCommand;
use Zephir\Di\Singleton;
use Zephir\FileSystem\HardDisk;
use Zephir\Support\TestCase;
use function Zephir\unlink_recursive;

/**
 * Zephir\Test\Command\CompileCommandTest
 */
class CompileCommandTest extends TestCase
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
        Singleton::reset();
    }

    /**
     * Restore current directory, and clean config.json.
     *
     * @return void
     */
    public function tearDown()
    {
        if (getcwd() !== $this->pwd) {
            if (file_exists(getcwd() . '/.temp')) {
                unlink_recursive(getcwd() . '/.temp');
            }

            if (file_exists(getcwd() . '/ext')) {
                unlink_recursive(getcwd() . '/ext');
            }

            if (file_exists(getcwd() . '/compile.log')) {
                unlink(getcwd() . '/compile.log');
            }

            if (file_exists(getcwd() . '/compile-errors.log')) {
                unlink(getcwd() . '/compile-errors.log');
            }

            chdir($this->pwd);
        }
    }

    /**
     * @test
     * @dataProvider devModeProvider
     * @issue https://github.com/phalcon/zephir/issues/1520
     *
     * @param string $flag
     * @param string $cflags
     */
    public function shouldDetermineDevOption($flag, $cflags)
    {
        // TODO: Create a test for Windows
        if (\strtoupper(\substr(PHP_OS, 0, 3)) === 'WIN') {
            $this->markTestSkipped(
                'This test currently works on Linux systems only'
            );
        }

        chdir(ZEPHIRPATH . '/unit-tests/fixtures/devmode');

        $container = $this->createContainer(getcwd());

        $application = new Application(ZEPHIRPATH, $container);
        $application->add(new CompileCommand());

        $this->muteOutput($container);

        $command = $application->find('compile');
        $commandTester = new CommandTester($command);

        $commandTester->execute(
            [
                'command' => $command->getName(),
                $flag => true,
            ],
            ['verbosity' => OutputInterface::VERBOSITY_QUIET]
        );

        $this->assertRegexp("/CFLAGS='{$cflags}'/", file_get_contents('ext/config.nice'));
    }

    public function devModeProvider()
    {
        return [
            ['--no-dev', '-O2 -fvisibility=hidden -Wparentheses( -flto)? -DZEPHIR_RELEASE=1'],
            ['--dev', '-O0 -g3'],
        ];
    }

    /**
     * @param  Container $container
     * @return void
     */
    protected function muteOutput(Container $container)
    {
        $container->get('config')->set('silent', true);
    }

    /**
     * Create internal Zephir's container.
     *
     * @param  $basePath
     * @return Container
     */
    protected function createContainer($basePath)
    {
        $container = new Container();

        $filesystem = function () use ($basePath) {
            $adapter = new Local($basePath);
            return new HardDisk(
                new Filesystem($adapter, ['visibility' => 'public'])
            );
        };

        $container->share('filesystem', $filesystem);

        return $container;
    }
}
