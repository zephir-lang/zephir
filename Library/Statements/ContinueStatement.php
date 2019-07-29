<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;

/**
 * ContinueStatement.
 *
 * Continue statement
 */
class ContinueStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        if ($compilationContext->insideCycle) {
            $compilationContext->codePrinter->output('continue;');
        } else {
            throw new CompilerException("Cannot use 'continue' outside of a loop", $this->statement);
        }
    }
}
