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
 * An ArrayAccess container whose offsetGet() returns a freshly built object.
 *
 * The value is owned by whoever receives it: nothing here keeps a reference to
 * it, which is the whole point. A container that cached the value would keep it
 * alive and hide the defect.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class Issue2682ProbeContainer implements ArrayAccess
{
    /** Set by Issue2682Probe::__destruct(). */
    public bool $destroyed = false;

    public function offsetExists(mixed $offset): bool
    {
        return true;
    }

    public function offsetGet(mixed $offset): mixed
    {
        return new Issue2682Probe($this);
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
    }

    public function offsetUnset(mixed $offset): void
    {
    }
}
