<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Test\CompilerFile;

use PHPUnit\Framework\TestCase;
use Psr\Log\Test\TestLogger;
use ReflectionException;
use Zephir\AliasManager;
use Zephir\Backend\Backend;
use Zephir\Compiler;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

final class CheckDependenciesTest extends TestCase
{
    /**
     * @throws ReflectionException
     */
    public function testExtendsClassThatDoesNotExist(): void
    {
        $logger = new TestLogger();
        $config = new Config();
        $backend = new Backend($config, 'kernel', 'templates');
        $disk = new HardDisk(realpath('..').'/tests/output');
        $compilerFactory = new Compiler\CompilerFileFactory($config, $disk, $logger);

        $compiler = new Compiler($config, $backend, new Manager(new Parser()), $disk, $compilerFactory);

        $compilerFile = new CompilerFile($config, new AliasManager(), $disk);
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
