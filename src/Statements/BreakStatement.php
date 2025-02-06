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
use Zephir\Exception\CompilerException;

class BreakStatement extends StatementAbstract
{
    /**
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        if ($compilationContext->insideCycle || $compilationContext->insideSwitch) {
            $compilationContext->codePrinter->output('break;');
        } else {
            throw new CompilerException("Cannot use 'break' outside of a loop", $this->statement);
        }
    }
}
