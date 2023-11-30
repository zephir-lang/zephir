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

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Operators\AbstractOperator;
use Zephir\Optimizers\EvalExpression;
use Zephir\Statements\LetStatement;

/**
 * Compiles ternary expressions
 */
class TernaryOperator extends AbstractOperator
{
    /**
     * Compile ternary operator.
     *
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        $returnVariable = $this->processTenary($compilationContext, $expression);

        $expr      = new EvalExpression();
        $condition = $expr->optimize($expression['left'], $compilationContext);
        $compilationContext->codePrinter->output('if (' . $condition . ') {');

        $compilationContext->codePrinter->increaseLevel();

        /**
         * Create an implicit 'let' operation to update the evaluated left operator.
         */
        $statement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['right'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('} else {');
        $compilationContext->codePrinter->increaseLevel();

        /**
         * Create an implicit 'let' operation to update the evaluated left operator.
         */
        $statement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['extra'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('variable', $returnVariable->getName(), $expression);
    }
}
