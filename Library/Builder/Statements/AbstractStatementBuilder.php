<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Statements;

/**
 * Class AbstractStatementBuilder.
 */
abstract class AbstractStatementBuilder
{
    /**
     * @return array
     */
    abstract public function get();
}
