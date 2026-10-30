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
 * A plain object whose property `p` a compound assignment reaches through.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747Holder
{
    public $p;

    public function __construct(mixed $p)
    {
        $this->p = $p;
    }
}
