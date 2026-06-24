<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Parser;

use function sprintf;

class Manager
{
    public const MINIMUM_PARSER_VERSION = '1.5.0';
    public const PARSER_HOME_PAGE       = 'https://github.com/zephir-lang/php-zephir-parser';

    public function __construct(protected Parser $parser)
    {
    }

    public function getParser(): Parser
    {
        return $this->parser;
    }

    /**
     * Check if a parser backend is available.
     *
     * Always true: Zephir now ships with a built-in pure-PHP parser, so the
     * optional `ext-zephir_parser` C extension is no longer required. The
     * extension remains supported as an optional accelerator.
     *
     * @return bool
     */
    public function isAvailable(): bool
    {
        return true;
    }

    public function requirements(): string
    {
        $template = <<<EOF


────────────────────────────────────────────────────────────
  The Zephir Parser extension %s.
  Note: Zephir no longer distributed with internal parser.
  %s
  To install latest stable Zephir Parser please refer to:
  %s
────────────────────────────────────────────────────────────
EOF;

        if ($this->parser->isAvailable()) {
            $add = sprintf("Minimum required Zephir Parser version: %s\n", self::MINIMUM_PARSER_VERSION);
        } else {
            $add = '';
        }

        return sprintf(
            $template,
            $this->parser->isAvailable() ? 'is outdated' : 'is not loaded',
            $add,
            self::PARSER_HOME_PAGE
        );
    }
}
