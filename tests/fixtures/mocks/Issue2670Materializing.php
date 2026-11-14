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
 * A lazy proxy: `__isset()` materialises the property by writing it, which
 * PHP 8.6 reads back for `empty()` instead of calling `__get()` (GH-12695).
 * Every magic call is recorded, so a test can compare the sequence.
 */
class Issue2670Materializing
{
    public $value;

    public array $calls = [];

    public function __construct(private mixed $materialized)
    {
        unset($this->value);
    }

    public function __isset(string $name): bool
    {
        $this->calls[] = '__isset';
        $this->$name   = $this->materialized;

        return true;
    }

    public function __get(string $name): mixed
    {
        $this->calls[] = '__get';

        return 'from __get';
    }
}
