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

use Stub\Issue2656;

/**
 * offsetExists() clears the property the fetch is reading through, which drops
 * the last reference to this container. zend_call_function() does not take a
 * reference for the call frame, which is why PHP's own
 * zend_std_read_dimension() does GC_ADDREF(object) / OBJ_RELEASE(object)
 * around both userland calls; without it kernel/array.c calls offsetGet() on a
 * freed object.
 *
 * The destructor is the assertion. Held correctly, this container cannot be
 * destroyed before offsetGet() runs, so offsetGet() reports 'alive'. Held
 * borrowed, __destruct() has already run by then and it reports 'freed'
 * (assuming the freed slot is still readable at all).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */
class Issue2656SelfDestroying implements ArrayAccess
{
    public const FLAG = 'issue2656_destructed';

    public function __construct(private ?Issue2656 $owner)
    {
        $GLOBALS[self::FLAG] = false;
    }

    public function __destruct()
    {
        $GLOBALS[self::FLAG] = true;
    }

    public function offsetExists(mixed $offset): bool
    {
        $owner       = $this->owner;
        $this->owner = null;
        $owner->setContainer(null);

        return true;
    }

    public function offsetGet(mixed $offset): mixed
    {
        return $GLOBALS[self::FLAG] ? 'freed' : 'alive';
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
    }

    public function offsetUnset(mixed $offset): void
    {
    }
}
