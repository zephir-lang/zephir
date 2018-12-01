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

use Zephir\Compiler;
use Zephir\FileSystem\FileSystemInterface;
use function Zephir\unlink_recursive;

class ProtoDirTest extends KernelTestCase
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

            chdir($this->pwd);
        }

        parent::tearDown();
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1758
     */
    public function shouldGenerateValidCodeWithInheritanceOfPrototypeInterfaces()
    {
        if (\PHP_VERSION_ID < 70000) {
            $this->markTestSkipped(
                'This test is designed to use for PHP < 7.0.0'
            );
        }

        $this->generate('ZendEngine3');

        $this->assertSame(
            implode(PHP_EOL, file('connectionexception.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/protodir/connectionexception.zep.h', FILE_IGNORE_NEW_LINES)),
            'Failed asserting that connectionexception.h and ext/protodir/connectionexception.zep.h are identical.'
        );
    }

    protected function generate($backend)
    {
        chdir(\constant('ZEPHIRPATH').'/unit-tests/fixtures/protodir');
        putenv('ZEPHIR_BACKEND='.$backend);

        self::bootKernel();

        $container = self::$kernel->getContainer();

        $compilerFs = $container->get(FileSystemInterface::class);
        $compilerFs->setBasePath(self::$kernel->getCacheDir());

        $this->muteOutput($container);

        $compiler = $container->get(Compiler::class);

        $reflection = new \ReflectionClass(Compiler::class);
        $loadedPrototypes = $reflection->getProperty('loadedPrototypes');
        $loadedPrototypes->setAccessible(true);
        $loadedPrototypes->setValue($compiler, false);

        $compiler->generate(true);
    }
}
