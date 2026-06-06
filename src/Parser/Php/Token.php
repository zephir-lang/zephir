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
        /**
         * Snapshots of the scanner's saved `class_*`/`method_*` state at the
         * time this token was produced. class/interface and function/method
         * nodes are stamped from the *lookahead* token's snapshot, because the
         * C builders read these mutable fields at reduce time — and a following
         * `class`/`function` keyword scanned as lookahead overwrites them.
         */
        public int $classLine = 0,
        public int $classChar = 0,
        public int $methodLine = 0,
        public int $methodChar = 0,
    ) {
    }
}
