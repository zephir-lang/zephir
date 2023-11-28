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
use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Statements\LetStatement;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

class AndOperator extends LogicalBaseOperator
{
    protected string $bitOperator = '&&';
    protected string $operator    = '&&';

    /**
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
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

        $assignExprLeft = match ($left->getType()) {
            Types::T_INT,
            Types::T_BOOL,
            Types::T_CHAR,
            Types::T_DOUBLE,
            Types::T_UINT,
            Types::T_UCHAR    => [
                'type'  => $left->getType(),
                'value' => $left->getCode(),
            ],
            Types::T_VARIABLE => [
                'type'  => 'variable',
                'value' => $left->getCode(),
            ],
            Types::T_NULL     => [
                'type'  => 'null',
                'value' => null,
            ],
            default           => throw new CompilerException(
                $left->getType(),
                $expression['left']
            ),
        };

        /**
         * Create an implicit 'let' operation to update the evaluated left operator.
         */
        $statement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $flagVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $assignExprLeft,
                    'file'        => $expression['left']['file'],
                    'line'        => $expression['left']['line'],
                    'char'        => $expression['left']['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->output($this->getOutput($flagVariable));
        $compilationContext->codePrinter->increaseLevel();

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly($this->readOnly);
        $right = $rightExpr->compile($compilationContext);

        $assignExprRight = match ($right->getType()) {
            Types::T_INT,
            Types::T_BOOL,
            Types::T_CHAR,
            Types::T_DOUBLE,
            Types::T_UINT,
            Types::T_UCHAR    => [
                'type'  => $right->getType(),
                'value' => $right->getCode(),
            ],
            Types::T_VARIABLE => [
                'type'  => 'variable',
                'value' => $right->getCode(),
            ],
            Types::T_NULL     => [
                'type'  => 'null',
                'value' => null,
            ],
            default           => throw new CompilerException(
                $right->getType(),
                $expression['right']
            ),
        };

        /**
         * Create an implicit 'let' operation to update the evaluated right operator.
         */
        $statement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $flagVariable->getName(),
                    'operator'    => 'assign',
                    'expr'        => $assignExprRight,
                    'file'        => $expression['right']['file'],
                    'line'        => $expression['right']['line'],
                    'char'        => $expression['right']['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('bool', $flagVariable->getName(), $expression);
    }

    /**
     * @param Variable $variable
     *
     * @return string
     */
    protected function getOutput(Variable $variable): string
    {
        return 'if (!(' . $variable->getName() . ')) {';
    }
}
