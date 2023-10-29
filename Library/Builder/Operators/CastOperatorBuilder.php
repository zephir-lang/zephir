<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Builder\Operators;

use Zephir\Builder\FunctionCallBuilder;

/**
 * Allows to manually build a 'cast' operator AST node
 */
class CastOperatorBuilder extends AbstractOperatorBuilder
{
    protected string $leftOperand;

    protected FunctionCallBuilder $rightOperand;

    public function __construct(
        string $left,
        FunctionCallBuilder $right,
        string $file = null,
        int $line = 0,
        int $char = 0
    ) {
        $this->leftOperand = $left;
        $this->rightOperand = $right;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    public function get(): array
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
