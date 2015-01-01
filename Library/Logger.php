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

namespace Zephir;

/**
 * Logger
 *
 * Entrypoint for warnings/notices/errors generated in compilation
 */
class Logger
{
    private static $files = array();

    /**
     * Stderr handler
     */
    protected $handler;

    /**
     * @var Config
     */
    protected $config;

    /**
     * Logger constructor
     *
     * @param Config $config
     */
    public function __construct(Config $config)
    {
        $this->config = $config;
    }

    /**
     * Changes a warning status on/off
     *
     * @param string $type
     * @param boolean $value
     */
    public function set($type, $value)
    {
        $this->config->set($type, $value, 'warnings');
    }

    /**
     * Sends a warning to the logger
     *
     * @param $message
     * @param $type
     * @param $node
     * @return bool
     * @throws CompilerException
     */
    public function warning($message, $type, $node)
    {
        if (!$this->config->get('silent')) {
            if (!$this->config->get($type, 'warnings')) {
                return false;
            }

            $warning  = 'Warning: ' . $message;
            if (!isset($node['file'])) {
                $warning .= ' in unknown on 0';
            } else {
                $warning .= ' in ' . $node['file'] . ' on ' . $node['line'];
            }
            $warning .= ' [' . $type . ']' . PHP_EOL;
            $warning .= PHP_EOL;
            if (isset($node['file'])) {
                if (!isset($_files[$node['file']])) {
                    if (file_exists($node['file'])) {
                        $lines = file($node['file']);
                    } else {
                        $lines = array();
                    }
                    $_files[$node['file']] = $lines;
                } else {
                    $lines = $_files[$node['file']];
                }
                if (isset($lines[$node['line'] - 1])) {
                    $line = $lines[$node['line'] - 1];
                    $warning .= "\t" . str_replace("\t", " ", $line);
                    if (($node['char'] - 1) > 0) {
                        $warning .= "\t" . str_repeat("-", $node['char'] - 1) . "^" . PHP_EOL;
                    }
                }
                $warning .= PHP_EOL;
            }

            if (!$this->handler) {
                $this->handler = STDERR;
            }

            fprintf($this->handler, "%s", Color::warning($warning));

            return true;
        }

        return false;
    }

    /**
     * Outputs a message to stdout if the silent flag is not enabled
     *
     * @param $message
     * @return bool
     */
    public function output($message)
    {
        if (!$this->config->get('silent')) {
            echo $message . PHP_EOL;
            return true;
        }
        return false;
    }
}
