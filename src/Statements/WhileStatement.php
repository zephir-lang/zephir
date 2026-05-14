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
 * While statement, the same as in PHP/C
 */
class WhileStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        /**
         * Compound conditions can be evaluated in a single line of the C-code
         */
        $compilationContext->codePrinter->output('while (1) {');
        $compilationContext->codePrinter->increaseLevel();

        /**
         * Variables are initialized in a different way inside loops
         */
        ++$compilationContext->insideCycle;

        $expr                 = new EvalExpression();
        $condition            = $expr->optimize($this->statement['expr'], $compilationContext);
        $this->evalExpression = $expr;

        $compilationContext->codePrinter->output('if (!(' . $condition . ')) {');
        $compilationContext->codePrinter->output("\t" . 'break;');
        $compilationContext->codePrinter->output('}');
        $compilationContext->codePrinter->decreaseLevel();

        /**
         * Compile statements in the 'while' block
         */
        if (isset($this->statement['statements'])) {
            $st = new StatementsBlock($this->statement['statements']);
            $st->isLoop(true);
            $st->compile($compilationContext);
        }

        /**
         * Restore the cycle counter
         */
        --$compilationContext->insideCycle;

        $compilationContext->codePrinter->output('}');
    }
}
