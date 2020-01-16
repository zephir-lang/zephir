<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
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
 * Unlikely.
 *
 * Adds a branch prediction hint when evaluating an expression
 */
class UnlikelyOperator extends BaseOperator
{
    /***
     * Compile unlikely operator
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'unlikely' expression", $expression['left']);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        if ('bool' == $left->getType()) {
            return new CompiledExpression('bool', 'UNEXPECTED('.$left->getCode().')', $expression);
        }

        if ('variable' == $left->getType()) {
            $variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
            switch ($variable->getType()) {
                case 'bool':
                    return new CompiledExpression('bool', 'UNEXPECTED('.$variable->getName().')', $expression);

                default:
                    $compilationContext->headersManager->add('kernel/operators');
                    $symbol = $compilationContext->backend->getVariableCode($variable);

                    return new CompiledExpression('bool', 'UNEXPECTED(zephir_is_true('.$symbol.'))', $expression);
            }
        }

        throw new CompilerException("Cannot use expression type: '".$left->getType()."' in 'unlikely' operator", $expression['left']);
    }
}
