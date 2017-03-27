<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | https://zephir-lang.com/license.html                                     |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Test\CompilerFile;

use Zephir\Config;
use Zephir\Compiler;
use Zephir\CompilerFile as sUT;
use Zephir\Backends\ZendEngine2\Backend;

class CheckDependenciesTest extends \PHPUnit_Framework_TestCase
{
    public function testExtendsClassThatDoesNotExist()
    {
        $config   = new Config();
        $manager  = $this->getMockBuilder('Zephir\Parser\Manager')->disableOriginalConstructor()->getMock();
        $logger   = $this->getMockBuilder('Zephir\Logger')->disableOriginalConstructor()->getMock();
        $compiler = new Compiler($config, $logger, new Backend($config), $manager);
        $sUT      = new sUT('myClass', 'myClass.zep', $config, $logger);

        $sUT->preCompileInterface(
            'myNamespace',
            ['name' => 'myClass', 'extends' => [['value' => 'doesNotExist']]],
            null
        );

        $logger->expects($this->once())->method('warning');

        $sUT->checkDependencies($compiler);
    }
}
