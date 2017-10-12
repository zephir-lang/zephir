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

namespace Zephir\Operators\Unary;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

class PlusOperator extends BaseOperator
{
    /**
     * Compile expression
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Missing left part of the expression");
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->_readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                return new CompiledExpression($left->getType(), '+' . $left->getCode(), $expression);

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariable($left->getCode());
                switch ($variable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression($variable->getType(), '+' . $variable->getName(), $expression);

                    case 'variable':
                        return new CompiledExpression('variable', $variable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot operate plus with variable of '" . $left->getType() . "' type");
                }
                break;

            default:
                throw new CompilerException("Cannot operate plus with '" . $left->getType() . "' type");
        }
    }
}
