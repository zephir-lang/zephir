<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

/**
 * Empty
 *
 * Checks if a variable is empty string or null
 */
class EmptyOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/operators');

        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'empty' expression", $expression['left']);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        if ($left->getType() != 'variable' && $left->getType() != 'array') {
            throw new CompilerException("'empty' operand only can be a variable", $expression['left']);
        }

        $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
        if (!$variableLeft->isVariable() && !$variableLeft->isString() && !$variableLeft->isArray()) {
            throw new CompilerException("Only dynamic/string variables can be used in 'empty' operators", $expression['left']);
        }

        return new CompiledExpression('bool', 'ZEPHIR_IS_EMPTY(' . $compilationContext->backend->getVariableCode($variableLeft) . ')', $expression);
    }
}
