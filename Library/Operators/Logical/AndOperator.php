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

namespace Zephir\Operators\Logical;

use Exception;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Statements\LetStatement;

class AndOperator extends LogicalBaseOperator
{
    protected string $operator = '&&';

    protected string $bitOperator = '&&';

    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws \Zephir\Exception
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        if (!isset($expression['left'])) {
            throw new Exception('Missing left part of the expression');
        }

        if (!isset($expression['right'])) {
            throw new Exception('Missing right part of the expression');
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        /**
         * This variable is used to check if the compound and expression is evaluated as true or false.
         */
        $flagVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext);

        switch ($left->getType()) {
            case 'int':
            case 'bool':
            case 'char':
            case 'double':
            case 'uint':
            case 'uchar':
                $assignExprLeft = [
                    'type' => $left->getType(),
                    'value' => $left->getCode(),
                ];
                break;
            case 'variable':
                $assignExprLeft = [
                    'type' => 'variable',
                    'value' => $left->getCode(),
                ];
                break;
            case 'null':
                $assignExprLeft = [
                    'type' => 'null',
                    'value' => null,
                ];
                break;
        }

        if (!isset($assignExprLeft)) {
            throw new CompilerException($left->getType(), $expression['left']);
        }

        /**
         * Create an implicit 'let' operation to update the evaluated left operator.
         */
        $statement = new LetStatement([
            'type' => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable' => $flagVariable->getName(),
                    'operator' => 'assign',
                    'expr' => $assignExprLeft,
                    'file' => $expression['left']['file'],
                    'line' => $expression['left']['line'],
                    'char' => $expression['left']['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->output('if ('.$flagVariable->getName().') {');
        $compilationContext->codePrinter->increaseLevel();

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly($this->readOnly);
        $right = $rightExpr->compile($compilationContext);

        switch ($right->getType()) {
            case 'int':
            case 'bool':
            case 'char':
            case 'double':
            case 'uint':
            case 'uchar':
                $assignExprRight = [
                    'type' => $right->getType(),
                    'value' => $right->getCode(),
                ];
                break;
            case 'variable':
                $assignExprRight = [
                    'type' => 'variable',
                    'value' => $right->getCode(),
                ];
                break;
            case 'null':
                $assignExprRight = [
                    'type' => 'null',
                    'value' => null,
                ];
                break;
        }

        if (!isset($assignExprRight)) {
            throw new CompilerException($right->getType(), $expression['right']);
        }

        /**
         * Create an implicit 'let' operation to update the evaluated right operator.
         */
        $statement = new LetStatement([
            'type' => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable' => $flagVariable->getName(),
                    'operator' => 'assign',
                    'expr' => $assignExprRight,
                    'file' => $expression['right']['file'],
                    'line' => $expression['right']['line'],
                    'char' => $expression['right']['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('bool', $flagVariable->getName(), $expression);
    }
}
