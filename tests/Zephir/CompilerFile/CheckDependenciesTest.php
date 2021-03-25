<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\CompilerFile;

use Psr\Log\Test\TestLogger;
use Zephir\AliasManager;
use Zephir\Compiler;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\Test\KernelTestCase;

final class CheckDependenciesTest extends KernelTestCase
{
    public function testExtendsClassThatDoesNotExist(): void
    {
        self::bootKernel(['config_files' => [__DIR__.'/../../config.yml']]);

        $logger = new TestLogger();

        /**
         * @var Compiler
         * @var Config              $config
         * @var FileSystemInterface $fs
         */
        $compiler = self::$kernel->getContainer()->get(Compiler::class);
        $config = self::$kernel->getContainer()->get(Config::class);
        $fs = self::$kernel->getContainer()->get(FileSystemInterface::class);

        $compilerFile = new CompilerFile($config, new AliasManager(), $fs);

        $compilerFile->setClassName('myClass');
        $compilerFile->setFilePath('myClass.zep');
        $compilerFile->setLogger($logger);

        $compilerFile->preCompileInterface(
            'myNamespace',
            ['name' => 'myClass', 'extends' => [['value' => 'doesNotExist']]],
            null
        );

        $compilerFile->checkDependencies($compiler);

        $this->assertTrue(
            $logger->hasWarning(
                'Cannot locate class "doesNotExist" when extending interface "myNamespace\myClass"'
            )
        );
    }
}
