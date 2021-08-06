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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * Empty.
 *
 * Checks if a variable is empty string or null
 */
class EmptyOperator extends BaseOperator
{
    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $compilationContext->headersManager->add('kernel/operators');

        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'empty' expression", $expression['left']);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        if ('variable' != $left->getType() && 'array' != $left->getType()) {
            throw new CompilerException("'empty' operand only can be a variable", $expression['left']);
        }

        $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
        if (!$variableLeft->isVariable() && !$variableLeft->isString() && !$variableLeft->isArray()) {
            throw new CompilerException("Only dynamic/string variables can be used in 'empty' operators", $expression['left']);
        }

        return new CompiledExpression('bool', 'ZEPHIR_IS_EMPTY('.$compilationContext->backend->getVariableCode($variableLeft).')', $expression);
    }
}
