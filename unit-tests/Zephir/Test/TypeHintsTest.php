<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use Zephir\Backends\ZendEngine2\Backend as Zend2;
use Zephir\Backends\ZendEngine3\Backend as Zend3;
use Zephir\Commands\CommandGenerate;
use Zephir\Commands\Manager as CommandsManager;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Logger;
use Zephir\Parser;
use Zephir\Parser\Manager as ParserManager;
use Zephir\Support\TestCase;

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

            chdir($this->pwd);
        }
    }

    protected function generate($backend)
    {
        chdir(ZEPHIRPATH . '/unit-tests/fixtures/typehints');

        try {
            $config = new Config();
            $logger = new Logger($config);
            $backend = new $backend($config);
            $parser = new ParserManager(new Parser(), $logger);
            $compiler = new Compiler($config, $logger, $backend, $parser);
            $command = new CommandGenerate(new CommandsManager);
            $compiler->generate($command);
        } catch (\Exception $e) {
            $this->fail($e->getMessage());
        }
    }

    public function shouldSpecifyCorrectArgumentInformationForZendEngine2()
    {
        $this->generate(Zend2::class);

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
        $this->generate(Zend3::class);

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
}
