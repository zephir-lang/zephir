<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Parser;

use Zephir\Logger;
use Zephir\Parser;

/**
 * Zephir\Parser\Manager
 */
class Manager
{
    const MINIMUM_PARSER_VERSION = '1.1.0';

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
     * @param Parser $parser The Zephir Parser
     * @param Logger $logger The Zephir Logger
     */
    public function __construct(Parser $parser, Logger $logger)
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
        return $this->parser->isAvailable() &&
            version_compare(self::MINIMUM_PARSER_VERSION, $this->parser->getVersion(), '<=');
    }

    /**
     * @return null|string
     */
    public function getParserVersion()
    {
        return $this->parser->getVersion();
    }

    public function requirements()
    {
        $temlate = <<<EOF


────────────────────────────────────────────────────────────
  The Zephir Parser extension %s.
  Note: Zephir no longer distributed with internal parser.
  %s
  To install latest stable Zephir Parser please refer to:
  https://github.com/phalcon/php-zephir-parser
────────────────────────────────────────────────────────────
EOF;

        if ($this->parser->isAvailable()) {
            $add = sprintf("Minimum required Zephir Parser version: %s\n", self::MINIMUM_PARSER_VERSION);
        } else {
            $add = "";
        }

        return sprintf($temlate, $this->parser->isAvailable() ? 'is outdated' : 'is not loaded', $add);
    }
}
