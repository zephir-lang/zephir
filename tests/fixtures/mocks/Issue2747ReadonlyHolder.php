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
 * PHP 8.1+ only: a compound assignment on an element of a readonly property
 * is a modification and must throw.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747ReadonlyHolder
{
    public readonly array $p;

    public function __construct()
    {
        $this->p = ['k' => 10];
    }
}
