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

class ContinueStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        if ($compilationContext->insideCycle) {
            $compilationContext->codePrinter->output('continue;');
        } else {
            throw new CompilerException("Cannot use 'continue' outside of a loop", $this->statement);
        }
    }
}
