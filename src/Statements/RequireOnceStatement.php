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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Expression;

/**
 * RequireOnceStatement.
 *
 * Require once statement is used to execute PHP scripts in a given path
 */
class RequireOnceStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $expression = [
            'type' => 'require_once',
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
