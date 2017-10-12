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
 * ParameterBuilder
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
     * Returns a builder definition
     *
     * @return array
     */
    public function get()
    {
        return array(
            'parameter' => $this->value->get()
        );
    }
}
