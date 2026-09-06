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

/**
 * An ArrayAccess container that answers every read with a fresh 64 byte string.
 *
 * A short or repeated literal would be interned or shared and the leak would
 * not show up in memory_get_usage(); str_repeat() allocates every time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class Issue2682FreshValueContainer implements ArrayAccess
{
    public function offsetExists(mixed $offset): bool
    {
        return true;
    }

    public function offsetGet(mixed $offset): mixed
    {
        return str_repeat('x', 64);
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
    }

    public function offsetUnset(mixed $offset): void
    {
    }
}
