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

class NotOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Missing left part of the expression", $expression);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->_readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'bool':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return new CompiledExpression('bool', '!(' . $left->getCode() . ')', $expression);

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
                switch ($variable->getType()) {
                    case 'bool':
                    case 'int':
                    case 'uint':
                    case 'long':
                        return new CompiledExpression('bool', '!' . $variable->getName(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbol = $compilationContext->backend->getVariableCode($variable);
                        return new CompiledExpression('bool', '!zephir_is_true(' . $symbol . ')', $expression);

                    default:
                        throw new CompilerException("Unknown type: " . $variable->getType(), $expression);
                }
                break;

            default:
                throw new CompilerException("Unknown type: " . $left->getType(), $expression);
        }
    }
}
