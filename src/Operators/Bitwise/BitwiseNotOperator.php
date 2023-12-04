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

namespace Zephir\Operators\Bitwise;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

class BitwiseNotOperator extends AbstractOperator
{
    /**
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'bool':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return new CompiledExpression('int', '~(' . $left->getCode() . ')', $expression);

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
                        return new CompiledExpression('int', '~' . $variable->getName(), $expression);

                    case 'variable':
                    case 'mixed':
                        $compilationContext->headersManager->add('kernel/operators');

                        return new CompiledExpression(
                            'int',
                            '~zephir_get_intval(' . $variable->getName() . ')',
                            $expression
                        );

                    default:
                        throw new CompilerException('Unknown type: ' . $variable->getType(), $expression);
                }

            default:
                throw new CompilerException('Unknown type: ' . $left->getType(), $expression);
        }
    }
}
