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
 * Records the offset offsetGet() is handed, so a test can assert on the key
 * the object actually received rather than only on the value it returned.
 *
 * 'offsetget' is a real entry: it is what kernel/array.c handed over once it
 * had released the offset string, because the method-name string allocated for
 * the offsetGet() call reused the freed slot.
 *
 * offsetExists() deliberately does not store $offset anywhere. Storing it
 * takes a reference, which keeps the string alive past the premature
 * zval_ptr_dtor() and hides the bug under test; a counter is enough to show
 * the call happened.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */
class Issue2656OffsetRecorder implements ArrayAccess
{
    /** @var array<int, mixed> offsets passed to offsetGet() */
    public array $seen = [];

    /** @var int number of offsetExists() calls */
    public int $existsCount = 0;

    /** @var array<array-key, mixed> */
    private array $data = [
        'cache.key' => 'hit',
        'offsetget' => 'CORRUPT',
        'absent.k1' => null,
        7           => 'seven',
    ];

    public function offsetExists(mixed $offset): bool
    {
        ++$this->existsCount;

        return isset($this->data[$offset]);
    }

    public function offsetGet(mixed $offset): mixed
    {
        $this->seen[] = $offset;

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
