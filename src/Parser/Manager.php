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
     * Oldest `ext-zephir_parser` whose grammar covers the syntax this Zephir
     * accepts. 2.7.0 added expressions in constant initializers and property
     * defaults (zephir#2061); 2.1.0 had added trait AST nodes (zephir#504).
     * Raise this in the same change that adds a grammar feature.
     */
    public const MINIMUM_PARSER_VERSION = '2.7.0';
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
     * fast path. An extension older than {@see MINIMUM_PARSER_VERSION} cannot
     * parse everything this Zephir accepts, so the built-in pure-PHP parser
     * must take over instead — it always matches the compiler it ships with.
     *
     * Both failure modes an outdated extension produces are covered by this:
     * silently dropping AST it does not know (2.0.x lost trait `use`
     * statements, zephir#504) and rejecting valid source outright (anything
     * before 2.7.0 reports a syntax error for an expression in a constant
     * initializer or property default, zephir#2061).
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
