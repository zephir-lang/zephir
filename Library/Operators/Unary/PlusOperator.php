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

namespace Zephir\Operators\Unary;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

class PlusOperator extends BaseOperator
{
    /**
     * Compile expression.
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        if (!isset($expression['left'])) {
            throw new CompilerException('Missing left part of the expression');
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                return new CompiledExpression($left->getType(), '+'.$left->getCode(), $expression);

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariable($left->getCode());
                switch ($variable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression($variable->getType(), '+'.$variable->getName(), $expression);

                    case 'variable':
                        return new CompiledExpression('variable', $variable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot operate plus with variable of '".$left->getType()."' type");
                }
                break;

            default:
                throw new CompilerException("Cannot operate plus with '".$left->getType()."' type");
        }
    }
}
