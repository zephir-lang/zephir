<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Operators;

/**
 * CastOperatorBuilder.
 *
 * Allows to manually build a 'cast' operator AST node
 */
class CastOperatorBuilder extends AbstractOperatorBuilder
{
    protected $leftOperand;

    protected $rightOperand;

    /**
     * @param $left
     * @param $right
     * @param null $file
     * @param int  $line
     * @param int  $char
     */
    public function __construct($left, $right, $file = null, $line = 0, $char = 0)
    {
        $this->leftOperand = $left;
        $this->rightOperand = $right;
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
            'type' => 'cast',
            'left' => $this->leftOperand,
            'right' => $this->rightOperand->get(),
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
