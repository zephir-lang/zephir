<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Builder;

/**
 * VariableBuilder
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
     * Returns a builder definition
     *
     * @return array
     */
    public function get()
    {
        return array(
            'type' => 'variable',
            'value' => $this->name
        );
    }
}
