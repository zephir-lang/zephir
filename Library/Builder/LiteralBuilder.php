<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * LiteralBuilder.
 *
 * Allows to manually build a literal expressions AST nodes
 */
class LiteralBuilder
{
    protected $type;

    protected $value;

    public function __construct($type, $value)
    {
        $this->type = $type;
        $this->value = $value;
    }

    public function get()
    {
        return [
            'type' => $this->type,
            'value' => $this->value,
        ];
    }
}
