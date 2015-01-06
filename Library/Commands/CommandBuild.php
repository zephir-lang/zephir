<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Commands;

/**
 * BuildCommand
 *
 * Generates/Builds/Install the extension
 */
class CommandBuild extends CommandAbstract
{

    /**
     * Commands provided by this command
     *
     * @return array|string
     */
    public function getCommand()
    {
        return 'build';
    }

    /**
     * Command usage
     *
     * @return string
     */
    public function getUsage()
    {
        return 'build';
    }

    /**
     * Returns the description of the command
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Generate/Compile/Install a Zephir extension';
    }
}
