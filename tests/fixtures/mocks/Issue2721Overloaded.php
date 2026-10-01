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
 * The plainest ArrayAccess container there is: offsetGet() hands back a value,
 * never a reference. That is the same position a <Ns>\Buffer element is in --
 * its elements are raw C scalars, so there is no zval to hand back either --
 * which makes this the oracle for any operation that needs an lvalue.
 *
 * Named rather than anonymous on purpose. PHP truncates an anonymous class
 * name at the NUL byte when it prints it inside the "Indirect modification of
 * overloaded element" notice, so get_class() does not match what the message
 * contains and the name cannot be normalised away when comparing transcripts.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2721
 */
final class Issue2721Overloaded implements ArrayAccess
{
    /** @var array<int, float> */
    private array $values;

    public function __construct(float $value)
    {
        $this->values = [0 => $value];
    }

    public function offsetExists(mixed $offset): bool
    {
        return isset($this->values[$offset]);
    }

    public function offsetGet(mixed $offset): mixed
    {
        return $this->values[$offset];
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
        $this->values[$offset] = $value;
    }

    public function offsetUnset(mixed $offset): void
    {
        unset($this->values[$offset]);
    }
}
