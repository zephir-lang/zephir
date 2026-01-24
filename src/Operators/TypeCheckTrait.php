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

namespace Zephir\Operators;

/**
 * Trait providing common type checking utilities for operators
 */
trait TypeCheckTrait
{
    /**
     * Check if the given type is an integer-like type.
     *
     * @param string $type
     * @return bool
     */
    protected function isIntegerType(string $type): bool
    {
        return in_array($type, ['int', 'uint', 'long', 'ulong'], true);
    }

    /**
     * Check if the given type is a char-like type.
     *
     * @param string $type
     * @return bool
     */
    protected function isCharType(string $type): bool
    {
        return in_array($type, ['char', 'uchar'], true);
    }

    /**
     * Check if the given type is a numeric type (int or double).
     *
     * @param string $type
     * @return bool
     */
    protected function isNumericType(string $type): bool
    {
        return $this->isIntegerType($type) || $type === 'double';
    }
}
