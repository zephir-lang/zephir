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

namespace Zephir;

use Zephir\Compiler\CompilerException;

/**
 * Logger
 *
 * Entrypoint for warnings/notices/errors generated in compilation.
 *
 * @package Zephir
 */
class Logger
{
    /**
     * The contents of the files that are involved in the log message.
     *
     * @var array
     */
    private $filesContent = [];

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
                $lines = $this->getFileContents($node['file']);

                if (isset($lines[$node['line'] - 1])) {
                    $line = $lines[$node['line'] - 1];
                    $warning .= "\t" . str_replace("\t", " ", $line);
                    if (($node['char'] - 1) > 0) {
                        $warning .= "\t" . str_repeat("-", $node['char'] - 1) . "^" . PHP_EOL;
                    }
                }

                $warning .= PHP_EOL;
            }

            fprintf(STDERR, "%s", Color::warning($warning));

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
            fwrite(STDOUT, $message . PHP_EOL);
            return true;
        }
        return false;
    }

    /**
     * Gets the contents of the files that are involved in the log message.
     *
     * @param string $file File path
     * @return array
     */
    protected function getFileContents($file)
    {
        if (!isset($this->filesContent[$file])) {
            $this->filesContent[$file] = file_exists($file) ? file($file) : [];
        }

        return $this->filesContent[$file];
    }
}
