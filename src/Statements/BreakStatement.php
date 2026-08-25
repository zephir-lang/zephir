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
        /**
         * A `switch` is lowered to labels and jumps, so leaving it means
         * jumping to its end label rather than emitting a C `break`.
         *
         * @see https://github.com/zephir-lang/zephir/issues/1704
         */
        $endLabel = $compilationContext->useSwitchEndLabel();
        if (null !== $endLabel) {
            $compilationContext->codePrinter->output('goto ' . $endLabel . ';');

            return;
        }

        if ($compilationContext->insideCycle) {
            $compilationContext->codePrinter->output('break;');

            return;
        }

        throw new CompilerException("Cannot use 'break' outside of a loop", $this->statement);
    }
}
