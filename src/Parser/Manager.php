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
use function version_compare;

class Manager
{
    /**
     * 2.1.0 introduced trait AST nodes. Older parsers silently drop unknown
     * class members, so accepting them would lose trait `use` statements.
     */
    public const MINIMUM_PARSER_VERSION = '2.1.0';
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

    /**
     * Whether a loaded C-extension parser is recent enough to serve as the
     * fast path. Extensions older than {@see MINIMUM_PARSER_VERSION} predate
     * trait AST nodes and silently drop unknown class members, so using them
     * would lose trait `use` statements without any error — the built-in
     * pure-PHP parser must take over instead.
     *
     * @param string|null $version `phpversion('zephir_parser')` or null when not loaded
     */
    public static function isExtensionUsable(?string $version): bool
    {
        return null !== $version
            && version_compare($version, self::MINIMUM_PARSER_VERSION, '>=');
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
