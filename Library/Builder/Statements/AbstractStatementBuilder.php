<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Builder\Statements;

/**
 * Class AbstractStatementBuilder
 * @package Zephir\Builder\Statements
 */
abstract class AbstractStatementBuilder
{
    /**
     * @return array
     */
    abstract public function get();
}
