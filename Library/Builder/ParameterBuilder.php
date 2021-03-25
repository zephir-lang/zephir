<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * ParameterBuilder.
 *
 * Allows to manually build a call parameters AST nodes
 */
class ParameterBuilder
{
    protected $value;

    public function __construct($value)
    {
        $this->value = $value;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        return [
            'parameter' => $this->value->get(),
        ];
    }
}
