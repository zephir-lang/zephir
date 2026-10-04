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

use ReflectionException;
use Zephir\Builder\Operators\UnaryOperatorBuilder;
use Zephir\Builder\Statements\IfStatementBuilder;
use Zephir\Builder\Statements\LetStatementBuilder;
use Zephir\Builder\StatementsBlockBuilder;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Operators\AbstractOperator;
use Zephir\Statements\IfStatement;
use Zephir\Statements\LetStatement;

/**
 * a ?: b
 *
 * Compiles short ternary expressions
 */
class ShortTernaryOperator extends AbstractOperator
{
    /**
     * Compile ternary operator.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        /**
         * The left operand is evaluated once, into a temporary that is also
         * the result, and the right operand only replaces it when it is falsy.
         *
         * The result never goes straight into the assigned variable: the right
         * operand may read that variable (`let a = b ?: a;`), and writing the
         * left operand first would clobber it.
         */
        $returnVariable = $compilationContext->symbolTable->getTempVariableForWrite(
            'variable',
            $compilationContext
        );
        $this->checkVariableTemporal($returnVariable);

        $position = [
            'file' => $expression['file'],
            'line' => $expression['line'],
            'char' => $expression['char'],
        ];

        $letLeft = new LetStatement((new LetStatementBuilder([
            'assign-type' => 'variable',
            'variable'    => $returnVariable->getName(),
            'operator'    => 'assign',
        ] + $position, $expression['left']))->get());
        $letLeft->compile($compilationContext);

        $ifBuilder = new IfStatementBuilder(
            new UnaryOperatorBuilder(
                'not',
                ['type' => 'variable', 'value' => $returnVariable->getName()] + $position
            ),
            new StatementsBlockBuilder([
                new LetStatementBuilder([
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                ] + $position, $expression['extra']),
            ])
        );

        $ifStatement = new IfStatement($ifBuilder->get());
        $ifStatement->compile($compilationContext);

        return new CompiledExpression('variable', $returnVariable->getName(), $expression);
    }
}
