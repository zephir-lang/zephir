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

use Zephir\Config;
use Zephir\Exception;
use Zephir\Logger;

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
    public function getUsage()
    {
        return sprintf(
            '%s [--theme-path=/path][--output-directory=/path][--theme-options={json}|/path]',
            $this->getCommand()
        );
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
     * @throws Exception
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
            if (isset($allowedArgs[$k])) {
                if (preg_match($allowedArgs[$k], $p)) {
                    $this->setParameter($k, $p);
                } else {
                    throw new Exception("Invalid value for argument '$k'");
                }
            } elseif (!in_array($k, ['parser-compiled'])) {
                throw new Exception("Invalid argument '$k' for api command'");
            }
        }

        parent::execute($config, $logger);
    }
}
