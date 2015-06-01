<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\EvalExpression;
use Zephir\Statements\LetStatement;

/**
 * Ternary
 *
 * Compiles ternary expressions
 */
class TernaryOperator extends BaseOperator
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

        $expr = new EvalExpression();
        $condition = $expr->optimize($expression['left'], $compilationContext);
        $compilationContext->codePrinter->output('if (' . $condition . ') {');

        $compilationContext->codePrinter->increaseLevel();

        /**
         * Create an implicit 'let' operation to update the evaluated left operator
         */
        $statement = new LetStatement(array(
            'type' => 'let',
            'assignments' => array(
                array(
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['right'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char'],
                )
            )
        ));
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('} else {');
        $compilationContext->codePrinter->increaseLevel();

        /**
         * Create an implicit 'let' operation to update the evaluated left operator
         */
        $statement = new LetStatement(array(
            'type' => 'let',
            'assignments' => array(
                array(
                    'assign-type' => 'variable',
                    'variable'    => $returnVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $expression['extra'],
                    'file'        => $expression['file'],
                    'line'        => $expression['line'],
                    'char'        => $expression['char'],
                )
            )
        ));
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('variable', $returnVariable->getName(), $expression);
    }
}
