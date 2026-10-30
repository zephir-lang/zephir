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
 * Holds a typed property an array element can be a reference to: the
 * compound assignment must verify the result against that type.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747TypedRefHolder
{
    public int $i = 1;
}
