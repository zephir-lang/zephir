<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir (https://zephir-lang.com/)             |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Commands;

/**
 * Zephir\Commands\CommandUsageTrait
 *
 * @package Zephir\Commands
 */
trait CommandUsageTrait
{
    /**
     * Prints formated command's usage.
     *
     * @return void
     */
    public function formatUsage()
    {
        echo 'Usage: ', PHP_EOL;
        echo "\tzephir ", $this->getUsage(), PHP_EOL, PHP_EOL;
        echo 'Help: ', PHP_EOL;
        echo sprintf("\t%s\n", $this->getDescription()), PHP_EOL;
    }
}
