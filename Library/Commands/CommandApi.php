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

namespace Zephir\Commands;

use Zephir\Config;
use Zephir\Logger;
use Zephir\Exception\InvalidArgumentException;

/**
 * Zephir\Commands\CommandApi
 *
 * Generates a HTML API based on the classes exposed in the extension.
 *
 * @package Zephir\Commands
 */
class CommandApi extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'api';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Generates a HTML API based on the classes exposed in the extension';
    }

    /**
     * {@inheritdoc}
     *
     * @param Config $config
     * @param Logger $logger
     * @throws InvalidArgumentException
     */
    public function execute(Config $config, Logger $logger)
    {
        if ($this->hasHelpOption()) {
            echo $this->getSynopsis();
            return;
        }

        $params = $this->parseArguments();

        $allowedArgs = [
            "theme-path"       => "@.+@",
            "output-directory" => "@.+@",
            "theme-options"    => "@.+@",
            "base-url"         => "@.+@",
        ];

        foreach ($params as $k => $p) {
            if (!isset($allowedArgs[$k])) {
                throw new InvalidArgumentException("Invalid argument '$k' for api command'");
            }

            if (!preg_match($allowedArgs[$k], $p)) {
                throw new InvalidArgumentException("Invalid value for argument '$k'");
            }

            $this->setParameter($k, $p);
        }

        parent::execute($config, $logger);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getUsage()
    {
        $template =<<<EOL
%s [options]

Description:
    The options are as follows:

    --theme-path=/path               The API theme to be used.

    --output-directory=/path         Output directory to generate theme.

    --theme-options={json}|/path     Theme options.
EOL;

        return sprintf($template, $this->getCommand());
    }
}
