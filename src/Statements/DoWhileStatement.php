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

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;

/**
 * DoWhile statement, the same as in PHP/C
 */
class DoWhileStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $exprRaw        = $this->statement['expr'];
        $codePrinter    = $compilationContext->codePrinter;
        $conditionLabel = 'do_cond_' . ++$compilationContext->doWhileLabelId;

        $codePrinter->output('do {');

        /**
         * Variables are initialized in a different way inside cycle
         */
        ++$compilationContext->insideCycle;
        $compilationContext->pushDoWhileTarget($conditionLabel);

        /**
         * Compile statements in the 'while' block
         */
        if (isset($this->statement['statements'])) {
            $st = new StatementsBlock($this->statement['statements']);
            $st->compile($compilationContext);
        }

        $isContinued = $compilationContext->popDoWhileTarget();

        $compilationContext->codePrinter->increaseLevel();

        /**
         * The condition below may print code before the test; a `continue`
         * lands here so that code runs on every iteration.
         */
        if ($isContinued) {
            $codePrinter->output($conditionLabel . ': ;');
        }

        $condition = (new EvalExpression())->optimize($exprRaw, $compilationContext);
        $compilationContext->codePrinter->decreaseLevel();

        /**
         * Restore the cycle counter
         */
        --$compilationContext->insideCycle;

        /**
         * Compound conditions can be evaluated in a single line of the C-code
         */
        $codePrinter->output('} while (' . $condition . ');');
    }
}
