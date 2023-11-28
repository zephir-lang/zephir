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

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

class NotOperator extends AbstractOperator
{
    /**
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        $this->checkLeftOperator($expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'bool':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return new CompiledExpression('bool', '!(' . $left->getCode() . ')', $expression);

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $left->getCode(),
                    $compilationContext,
                    $expression['left']
                );
                switch ($variable->getType()) {
                    case 'bool':
                    case 'int':
                    case 'uint':
                    case 'long':
                        return new CompiledExpression('bool', '!' . $variable->getName(), $expression);

                    case 'variable':
                    case 'mixed':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbol = $compilationContext->backend->getVariableCode($variable);

                        return new CompiledExpression('bool', '!zephir_is_true(' . $symbol . ')', $expression);

                    default:
                        throw new CompilerException('Unknown type: ' . $variable->getType(), $expression);
                }
                break;

            default:
                throw new CompilerException('Unknown type: ' . $left->getType(), $expression);
        }
    }
}
