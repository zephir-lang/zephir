<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;

abstract class IsTypeOptimizerAbstract extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (1 != \count($expression['parameters'])) {
            return false;
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ('IS_BOOL' == $this->getType()) {
            $condition = sprintf(
                '(Z_TYPE_P(%s) == IS_TRUE || Z_TYPE_P(%s) == IS_FALSE)',
                $resolvedParams[0],
                $resolvedParams[0]
            );
        } else {
            $condition = 'Z_TYPE_P('.$resolvedParams[0].') == '.$this->getType();
        }

        return new CompiledExpression('bool', $condition, $expression);
    }

    abstract protected function getType();
}
