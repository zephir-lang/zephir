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
use Zephir\Types;

/**
 * RangeInclusive.
 *
 * Inclusive range operator
 */
class RangeInclusiveOperator extends BaseOperator
{
    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'irange' expression", $expression['left']);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException("Invalid 'right' operand for 'irange' expression", $expression['right']);
        }

        $exprBuilder = Expression\Builder\BuilderFactory::getInstance();

        /**
         * Implicit type coercing.
         */
        $castBuilder = $exprBuilder->operators()->cast(Types::T_ARRAY, $exprBuilder->statements()
            ->functionCall('range', [$expression['left'], $expression['right']]));

        $expression = new Expression($castBuilder->build());
        $expression->setReadOnly($this->readOnly);

        return $expression->compile($compilationContext);
    }
}
