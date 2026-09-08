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
 * offsetExists() answers with a fresh heap string instead of a bool. A bool is
 * not refcounted, so it hides the fact that the isset helpers in
 * kernel/array.c never released the offsetExists() return value; a string
 * makes every dropped result cost 64 bytes.
 *
 * The tentative bool return type is opted out of, not violated silently.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */
class Issue2656RefcountedExists implements ArrayAccess
{
    /** @var array<array-key, mixed> */
    private array $data = [
        'cache.key' => 'hit',
        'absent.k1' => null,
        7           => 'seven',
    ];

    #[\ReturnTypeWillChange]
    public function offsetExists(mixed $offset)
    {
        return isset($this->data[$offset]) ? str_repeat('x', 64) : '';
    }

    public function offsetGet(mixed $offset): mixed
    {
        return $this->data[$offset] ?? null;
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
        if (null === $offset) {
            $this->data[] = $value;

            return;
        }

        $this->data[$offset] = $value;
    }

    public function offsetUnset(mixed $offset): void
    {
        unset($this->data[$offset]);
    }
}
