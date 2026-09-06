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
 * An ArrayAccess container that answers every read with a fresh array.
 *
 * The companion of Issue2682FreshValueContainer for the write-context path: a
 * by-reference call argument needs an array to push into, and the array has to
 * be built per call so an unreleased one shows up in memory_get_usage().
 *
 * Nothing here keeps a reference to what offsetGet() returns, and offsetSet()
 * is a no-op, which is what PHP's own semantics come to for this construct: the
 * write has no effect on the container.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class Issue2682FreshArrayContainer implements ArrayAccess
{
    public function offsetExists(mixed $offset): bool
    {
        return true;
    }

    public function offsetGet(mixed $offset): mixed
    {
        return [str_repeat('y', 64)];
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
    }

    public function offsetUnset(mixed $offset): void
    {
    }
}
