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
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Operators\AbstractOperator;

/**
 * Obtains the type of dynamic variable.
 */
class TypeOfOperator extends AbstractOperator
{
    /**
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws \ReflectionException
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'typeof' expression", $expression['left']);
        }

        $functionCall = BuilderFactory::getInstance()->statements()->functionCall('gettype', [$expression['left']]);
        $expression = new Expression($functionCall->build());

        return $expression->compile($compilationContext);
    }
}
