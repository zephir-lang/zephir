<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

/**
 * RequireStatement.
 *
 * Require statement is used to execute PHP scripts in a given path
 */
class RequireStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $expression = [
            'type' => 'require',
            'left' => $this->statement['expr'],
            'file' => $this->statement['file'],
            'line' => $this->statement['line'],
            'char' => $this->statement['char'],
        ];

        $expr = new Expression($expression);
        $expr->setExpectReturn(false, null);
        $expr->compile($compilationContext);
    }
}
