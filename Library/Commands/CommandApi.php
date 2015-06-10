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

use Zephir\Config;
use Zephir\Exception;
use Zephir\Logger;

/**
 * CommandApi
 *
 * Generates a HTML API based on the classes exposed in the extension
 */
class CommandApi extends CommandAbstract
{
    /**
     * Command provided by this command
     *
     * @return array|string
     */
    public function getCommand()
    {
        return 'api';
    }

    /**
     * Command usage
     *
     * @return string
     */
    public function getUsage()
    {
        return 'api [--theme-path=/path][--output-directory=/path][--theme-options={json}|/path]';
    }

    /**
     * Returns the description of the command
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Generates a HTML API';
    }

    public function execute(Config $config, Logger $logger)
    {

        $params = $this->parseArguments();

        $allowedArgs = array(
            "theme-path"       => "@.+@",
            "output-directory" => "@.+@",
            "theme-options"    => "@.+@",
            "base-url"         => "@.+@",
        );

        foreach ($params as $k => $p) {
            if (isset($allowedArgs[$k])) {
                if (preg_match($allowedArgs[$k], $p)) {
                    $this->setParameter($k, $p);
                } else {
                    throw new Exception("Invalid value for argument '$k'");
                }

            } else {
                throw new Exception("Invalid argument '$k''");
            }
        }

        parent::execute($config, $logger);
    }
}
