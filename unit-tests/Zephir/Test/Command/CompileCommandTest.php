<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Command;

use Symfony\Component\Console\Tester\CommandTester;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\Application;
use Zephir\Command\CompileCommand;
use Zephir\Di\Singleton;
use Zephir\Support\TestCase;
use function Zephir\unlink_recursive;

/**
 * Zephir\Test\Command\CompileCommandTest
 *
 * @package Zephir\Test\CompilerFile
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
            if (file_exists('.temp')) {
                unlink_recursive('.temp/');
            }

            if (file_exists('ext')) {
                unlink_recursive('ext/');
            }

            if (file_exists('compile.log')) {
                unlink('compile.log');
            }

            if (file_exists('compile-errors.log')) {
                unlink('compile-errors.log');
            }

            chdir($this->pwd);
        }
    }

    /**
     * @test
     * @dataProvider devModeProvider
     * @issue https://github.com/phalcon/zephir/issues/1520
     *
     * @param array  $mode
     * @param string $cflags
     */
    public function shouldDetermineDevOption(array $mode, $cflags)
    {
        // TODO: Create a test for Windows
        if (\strtoupper(\substr(PHP_OS, 0, 3)) === 'WIN') {
            $this->markTestSkipped(
                'This test currently works on Linux systems only'
            );
        }

        chdir(ZEPHIRPATH . '/unit-tests/fixtures/devmode');

        $application = new Application(ZEPHIRPATH);
        $application->add(new CompileCommand());

        Singleton::getDefault()->get('config')->set('silent', true);

        $command = $application->find('compile');
        $commandTester = new CommandTester($command);

        $commandTester->execute(
            ['command'  => $command->getName()] + $mode,
            ['verbosity' => OutputInterface::VERBOSITY_QUIET]
        );

        $this->assertRegexp("/CFLAGS='{$cflags}'/", file_get_contents('ext/config.nice'));
    }

    public function devModeProvider()
    {
        return [
            [['--no-dev' => true], '-O2 -fvisibility=hidden -Wparentheses -flto -DZEPHIR_RELEASE=1'],
            [['--dev' => true], '-O0 -g3'],
        ];
    }
}
