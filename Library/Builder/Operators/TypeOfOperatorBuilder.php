<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Operators;

/**
 * TypeOfOperatorBuilder.
 *
 * Allows to manually build a 'typeof' operator AST node
 */
class TypeOfOperatorBuilder extends AbstractOperatorBuilder
{
    protected $leftOperand;

    /**
     * @param $left
     * @param null $file
     * @param int  $line
     * @param int  $char
     */
    public function __construct($left, $file = null, $line = 0, $char = 0)
    {
        $this->leftOperand = $left;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        return [
            'type' => 'typeof',
            'left' => $this->leftOperand->get(),
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
