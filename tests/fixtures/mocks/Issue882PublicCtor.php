<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

/**
 * @see https://github.com/zephir-lang/zephir/issues/882
 */
class Issue882PublicCtor
{
    public $built = false;

    public function __construct()
    {
        $this->built = true;
    }
}
