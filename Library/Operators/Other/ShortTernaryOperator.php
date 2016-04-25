<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2016 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Other;

use Zephir\Builder\Operators\UnaryOperatorBuilder;
use Zephir\Builder\Statements\IfStatementBuilder;
use Zephir\Builder\Statements\LetStatementBuilder;
use Zephir\Builder\StatementsBlockBuilder;
use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\EvalExpression;
use Zephir\Statements\IfStatement;
use Zephir\Statements\LetStatement;

/**
 * ShortTernary
 *
 * a ?: b
 *
 * Compiles short ternary expressions
 */
class ShortTernaryOperator extends BaseOperator
{
    /**
     * Compile ternary operator
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        /**
         * This variable is used to check if the compound and expression is evaluated as true or false:
         * Ensure that newly allocated variables are local-only (setReadOnly)
         */
        $this->setReadOnly(false);
        $returnVariable = $this->getExpected($compilationContext, $expression, false);
        /* Make sure that passed variables (passed symbol variables) are promoted */
        $returnVariable->setLocalOnly(false);

        if ($returnVariable->getType() != 'variable' || $returnVariable->getName() == 'return_value') {
            $returnVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            if ($returnVariable->isTemporal()) {
                $returnVariable->skipInitVariant(2);
            }
        }

        $ifBuilder = new IfStatementBuilder(
            new UnaryOperatorBuilder(
                'not',
                $expression['left']
            ),
            new StatementsBlockBuilder(array(
                /**
                 * Create an implicit 'let' operation to update the evaluated right operator
                 */
                new LetStatementBuilder(array(
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['extra'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char']
                ), $expression['extra'])
            )),
            new StatementsBlockBuilder(array(
                /**
                 * Create an implicit 'let' operation to update the evaluated right operator
                 */
                new LetStatementBuilder(array(
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['left'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char']
                ), $expression['extra'])
            ))
        );

        $ifStatement = new IfStatement($ifBuilder->get());
        $ifStatement->compile($compilationContext);

        return new CompiledExpression('variable', $returnVariable->getName(), $expression);
    }
}
