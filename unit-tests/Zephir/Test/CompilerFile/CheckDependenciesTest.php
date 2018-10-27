<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\CompilerFile;

use Zephir\Config;
use Zephir\Compiler;
use Zephir\CompilerFile;
use Zephir\Backends\ZendEngine2\Backend;
use Zephir\Support\TestCase;
use Zephir\Parser\Manager;
use Zephir\Logger;

class CheckDependenciesTest extends TestCase
{
    public function testExtendsClassThatDoesNotExist()
    {
        /** @var Logger $logger */
        $logger = $this->getMockBuilder(Logger::class)->disableOriginalConstructor()->getMock();

        /** @var Manager $manager */
        $manager = $this->getMockBuilder(Manager::class)->disableOriginalConstructor()->getMock();

        $config = new Config();
        $compiler = new Compiler($config, $logger, new Backend($config), $manager);
        $compilerFile = new CompilerFile('myClass', 'myClass.zep', $config, $logger);

        $compilerFile->preCompileInterface(
            'myNamespace',
            ['name' => 'myClass', 'extends' => [['value' => 'doesNotExist']]],
            null
        );

        $logger->expects($this->once())->method('warning');

        $compilerFile->checkDependencies($compiler);
    }
}
