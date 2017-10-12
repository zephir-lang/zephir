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

namespace Zephir\Parser;

use Zephir\Logger;
use Zephir\Parser;

/**
 * Zephir\Parser\Manager
 */
class Manager
{
    /**
     * Zephir Parser
     * @var Parser
     */
    protected $parser;

    /**
     * Is the Zephir Parser enabled
     * @var bool
     */
    protected $parserEnabled = false;

    /**
     * Should we recompile the Zephir Parser
     * @var bool
     */
    protected $forceCompileParser = false;

    /**
     * The Zephir Logger
     * @var Logger
     */
    protected $logger;

    /**
     * Manager constructor.
     *
     * @param Parser $parser     The Zephir Parser
     * @param Logger $logger     The Zephir Logger
     * @param array  $parameters Additional parameters [Optional]
     */
    public function __construct(Parser $parser, Logger $logger, array $parameters = [])
    {
        $this->parser = $parser;
        $this->logger = $logger;
    }

    /**
     * Get Zephir Parser.
     *
     * @return Parser
     */
    public function getParser()
    {
        return $this->parser;
    }

    /**
     * Check if Zephir Parser available.
     *
     * @return bool
     */
    public function isAvailable()
    {
        return $this->parser->isAvailable();
    }
}
