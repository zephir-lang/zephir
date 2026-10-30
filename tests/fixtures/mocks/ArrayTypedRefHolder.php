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
 * Typed properties an array element can be a reference to. Writing an offset
 * through such a reference may turn null or false into an array only when
 * the property's type allows one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class ArrayTypedRefHolder
{
    public ?int $i = null;

    public ?array $a = null;

    public int|false $f = false;

    public mixed $m = null;
}
