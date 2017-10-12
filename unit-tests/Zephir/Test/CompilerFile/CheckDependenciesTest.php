<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
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
