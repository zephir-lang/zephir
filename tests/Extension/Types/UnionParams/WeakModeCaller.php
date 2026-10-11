<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Types\UnionParams;

/**
 * Calls a method from a file without `strict_types`, so arguments are passed
 * in coercive (weak) mode.
 */
final class WeakModeCaller
{
    public static function call(callable $method, array $arguments)
    {
        return $method(...$arguments);
    }
}
