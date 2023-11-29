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

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
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
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        $this->checkLeft($expression);
        $this->checkRight($expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        /**
         * This variable is used to check if the compound and expression is evaluated as true or false.
         */
        $flagVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext);

        $assignExprLeft = $this->getAssignmentExpression($left, $expression['left']);

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

        $assignExprRight = $this->getAssignmentExpression($right, $expression['right']);

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
        return 'if (' . $variable->getName() . ') {';
    }

    /**
     * @param CompiledExpression $left
     * @param array              $expression
     *
     * @return array
     */
    private function getAssignmentExpression(CompiledExpression $left, array $expression): array
    {
        return match ($left->getType()) {
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
                $left->getType(), $expression
            ),
        };
    }
}
