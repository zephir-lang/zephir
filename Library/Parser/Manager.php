<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Parser;

class Manager
{
    const MINIMUM_PARSER_VERSION = '1.1.0';

    /**
     * Zephir Parser.
     *
     * @var Parser
     */
    protected $parser;

    /**
     * Manager constructor.
     *
     * @param Parser $parser The Zephir Parser
     */
    public function __construct(Parser $parser)
    {
        $this->parser = $parser;
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
     * @return string|null
     */
    public function getParserVersion()
    {
        return $this->parser->getVersion();
    }

    public function requirements()
    {
        $template = <<<EOF


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
            $add = '';
        }

        return sprintf($template, $this->parser->isAvailable() ? 'is outdated' : 'is not loaded', $add);
    }
}
