<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * VariableBuilder.
 *
 * Allows to manually build a variable AST node
 */
class VariableBuilder
{
    protected $name;

    public function __construct($name)
    {
        $this->name = $name;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        return [
            'type' => 'variable',
            'value' => $this->name,
        ];
    }
}
