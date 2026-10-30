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
 * An uninitialized typed property: a read-write fetch must throw rather than
 * initialize it, which is what tells PHP's RW context from a plain write.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747TypedHolder
{
    public array $p;
}
