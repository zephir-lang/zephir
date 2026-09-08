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
 * The plain PHP counterpart of Stub\Issue2682::pushIntoDefault().
 *
 * Zephir compiles an array property default into one persistent table, refcount
 * 2 and IS_ARRAY_IMMUTABLE, shared by every instance until something writes to
 * it. PHP has no such table, but it does have the behaviour that matters here:
 * the push lands in this instance and leaves the next one alone.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
final class Issue2682DefaultsHolder
{
    protected array $defaults = ['bucket' => ['seed']];

    public function push(string $value): array
    {
        array_push($this->defaults['bucket'], $value);

        return $this->defaults;
    }
}
