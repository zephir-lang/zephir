<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * Likely.
 *
 * Adds a branch prediction hint when evaluating an expression
 */
class LikelyOperator extends BaseOperator
{
    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'likely' expression", $expression['left']);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        if ('bool' == $left->getType()) {
            return new CompiledExpression('bool', 'EXPECTED('.$left->getCode().')', $expression);
        }

        if ('variable' == $left->getType()) {
            $variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
            switch ($variable->getType()) {
                case 'bool':
                    return new CompiledExpression('bool', 'EXPECTED('.$variable->getName().')', $expression);

                default:
                    $compilationContext->headersManager->add('kernel/operators');
                    $symbol = $compilationContext->backend->getVariableCode($variable);

                    return new CompiledExpression('bool', 'UNEXPECTED(zephir_is_true('.$symbol.'))', $expression);
            }
        }

        throw new CompilerException("Cannot use expression type: '".$left->getType()."' in 'likely' operator", $expression['left']);
    }
}
