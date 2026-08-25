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

class ContinueStatement extends StatementAbstract
{
    /**
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        /**
         * Inside a `switch`, `continue` is equivalent to `break` - PHP warns
         * about it and treats it as one, and Zephir has no `continue N` with
         * which to name an outer loop.
         *
         * @see https://github.com/zephir-lang/zephir/issues/1704
         */
        $endLabel = $compilationContext->useSwitchEndLabel();
        if (null !== $endLabel) {
            $compilationContext->codePrinter->output('goto ' . $endLabel . ';');

            return;
        }

        if ($compilationContext->insideCycle) {
            $compilationContext->codePrinter->output('continue;');

            return;
        }

        throw new CompilerException("Cannot use 'continue' outside of a loop", $this->statement);
    }
}
