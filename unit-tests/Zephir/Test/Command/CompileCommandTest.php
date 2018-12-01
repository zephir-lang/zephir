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

use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Tester\CommandTester;
use Zephir\Console\Application;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\Test\KernelTestCase;
use function Zephir\unlink_recursive;

/**
 * Zephir\Test\Command\CompileCommandTest.
 */
class CompileCommandTest extends KernelTestCase
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
        if (getcwd() !== $this->pwd) {
            $dotZephir = \dirname(\dirname(self::$kernel->getCacheDir()));
            if (file_exists($dotZephir)) {
                unlink_recursive($dotZephir);
            }

            if (file_exists(getcwd().'/ext')) {
                unlink_recursive(getcwd().'/ext');
            }

            if (file_exists(getcwd().'/compile.log')) {
                unlink(getcwd().'/compile.log');
            }

            if (file_exists(getcwd().'/compile-errors.log')) {
                unlink(getcwd().'/compile-errors.log');
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
        if ('WIN' === \strtoupper(\substr(PHP_OS, 0, 3))) {
            $this->markTestSkipped(
                'This test currently works on Linux systems only'
            );
        }

        chdir(\constant('ZEPHIRPATH').'/unit-tests/fixtures/devmode');

        self::bootKernel();

        $container = self::$kernel->getContainer();

        $application = $container->get(Application::class);

        $compilerFs = $container->get(FileSystemInterface::class);
        $compilerFs->setBasePath(self::$kernel->getCacheDir());

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
}
