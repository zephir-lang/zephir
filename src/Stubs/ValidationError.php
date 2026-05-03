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

namespace Zephir\Stubs;

/**
 * Carries diagnostic information about a malformed PHPStan/Psalm type
 * expression detected by PhpStanTagValidator.
 */
final class ValidationError
{
    public function __construct(
        public readonly string $message,
        public readonly int $column,
    ) {
    }
}
