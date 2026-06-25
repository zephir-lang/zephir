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

namespace Zephir\Zept;

/**
 * Outcome of running a single `.zept` case.
 *
 * `status` is one of the class constants. `expected`/`actual` are populated on
 * FAIL so callers can render a diff; `reason` carries the human explanation for
 * FAIL/SKIP/ERROR.
 */
final class ZeptResult
{
    public const PASS  = 'pass';
    public const FAIL  = 'fail';
    public const SKIP  = 'skip';
    public const ERROR = 'error';

    public function __construct(
        public string $status,
        public string $reason = '',
        public string $expected = '',
        public string $actual = '',
    ) {
    }

    public static function pass(): self
    {
        return new self(self::PASS);
    }

    public static function fail(string $reason, string $expected, string $actual): self
    {
        return new self(self::FAIL, $reason, $expected, $actual);
    }

    public static function skip(string $reason): self
    {
        return new self(self::SKIP, $reason);
    }

    public static function error(string $reason): self
    {
        return new self(self::ERROR, $reason);
    }
}
