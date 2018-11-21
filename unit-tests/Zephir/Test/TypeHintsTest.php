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

use League\Container\Container;
use League\Container\ReflectionContainer;
use League\Flysystem\Adapter\Local;
use League\Flysystem\Filesystem;
use Zephir\Application;
use Zephir\Compiler;
use Zephir\Di\Singleton;
use Zephir\FileSystem\HardDisk;
use Zephir\Support\TestCase;
use function Zephir\unlink_recursive;

class TypeHintsTest extends TestCase
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
        chdir(ZEPHIRPATH . '/unit-tests/fixtures/typehints');

        try {
            putenv('ZEPHIR_BACKEND=' . $backend);

            $container = $this->createContainer(getcwd());
            $container->delegate(new ReflectionContainer());

            new Application(ZEPHIRPATH, $container);
            $this->muteOutput($container);

            /** @var Compiler $compiler */
            $compiler = $container->get(Compiler::class);
            $compiler->generate(true);
        } catch (\Exception $e) {
            $this->fail($e->getMessage());
        }
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
