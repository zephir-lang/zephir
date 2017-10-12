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
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;

/**
 * Likely
 *
 * Adds a branch prediction hint when evaluating an expression
 */
class LikelyOperator extends BaseOperator
{
    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'likely' expression", $expression['left']);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        if ($left->getType() == 'bool') {
            return new CompiledExpression('bool', 'EXPECTED(' . $left->getCode() . ')', $expression);
        }

        if ($left->getType() == 'variable') {
            $variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
            switch ($variable->getType()) {
                case 'bool':
                    return new CompiledExpression('bool', 'EXPECTED(' . $variable->getName() . ')', $expression);

                default:
                    $compilationContext->headersManager->add('kernel/operators');
                    $symbol = $compilationContext->backend->getVariableCode($variable);
                    return new CompiledExpression('bool', 'UNEXPECTED(zephir_is_true(' . $symbol . '))', $expression);
            }
        }

        throw new CompilerException("Cannot use expression type: '" . $left->getType() . "' in 'likely' operator", $expression['left']);
    }
}
