<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Operators\Other;

use Zephir\Expression;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Operators\BaseOperator;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Builder\BuilderFactory;

/**
 * TypeOf
 *
 * Obtains the type of a dynamic variable
 */
class TypeOfOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression
     * @throws \Zephir\Exception\CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'typeof' expression", $expression['left']);
        }

        $functionCall = BuilderFactory::getInstance()->statements()->functionCall('gettype', [$expression['left']]);
        $expression = new Expression($functionCall->build());
        return $expression->compile($compilationContext);
    }
}
