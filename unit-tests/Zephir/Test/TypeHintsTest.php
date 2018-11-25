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

class TypeHintsTest extends KernelTestCase
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
    }

    /** @test */
    public function shouldSpecifyCorrectArgumentInformationForZendEngine2()
    {
        $this->generate('ZendEngine2');

        $this->assertSame(
            implode(PHP_EOL, file('expected2.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints.c', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_args2.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/args.zep.h', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_retval2.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/retval.zep.h', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_both2.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/both.zep.h', FILE_IGNORE_NEW_LINES))
        );
    }

    /** @test */
    public function shouldSpecifyCorrectArgumentInformationForZendEngine3()
    {
        $this->generate('ZendEngine3');

        $this->assertSame(
            implode(PHP_EOL, file('expected3.c', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints.c', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_args3.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/args.zep.h', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_retval3.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/retval.zep.h', FILE_IGNORE_NEW_LINES))
        );

        $this->assertSame(
            implode(PHP_EOL, file('expected_both3.h', FILE_IGNORE_NEW_LINES)),
            implode(PHP_EOL, file('ext/typehints/both.zep.h', FILE_IGNORE_NEW_LINES))
        );
    }

    protected function generate($backend)
    {
        chdir(\constant('ZEPHIRPATH').'/unit-tests/fixtures/typehints');
        putenv('ZEPHIR_BACKEND='.$backend);

        self::bootKernel();

        $container = self::$kernel->getContainer();

        $compilerFs = $container->get(FileSystemInterface::class);
        $compilerFs->setBasePath(self::$kernel->getCacheDir());

        $this->muteOutput($container);

        $compiler = $container->get(Compiler::class);
        $compiler->generate(true);
    }

    /**
     * @param ContainerInterface $container
     */
    protected function muteOutput(ContainerInterface $container)
    {
        $container->get(Config::class)->set('silent', true);
    }
}
