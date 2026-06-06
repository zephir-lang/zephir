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

namespace Zephir\Parser\Php;

/**
 * A single lexer token.
 *
 * `line`/`char` record the scanner position *immediately after this token was
 * lexed* (the C extension's `active_line`/`active_char` state at that moment).
 * That is exactly the position the parser stamps onto a node when this token is
 * the current lookahead — see the position-fidelity invariant ported from
 * `scanner.re`/`base.c`.
 */
final class Token
{
    public function __construct(
        public int $opcode,
        public ?string $value,
        public int $line,
        public int $char,
    ) {
    }
}
