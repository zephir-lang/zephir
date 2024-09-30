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

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;

use function count;
use function sprintf;

abstract class IsTypeOptimizerAbstract extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|null
     *
     * @throws Exception
     */
    public function optimize(array $expression, Call $call, CompilationContext $context): ?CompiledExpression
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) !== 1) {
            return null;
        }

        $resolvedParam = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression)[0];

        if ('IS_BOOL' === $this->getType()) {
            $condition = sprintf(
                '(Z_TYPE_P(%s) == IS_TRUE || Z_TYPE_P(%s) == IS_FALSE)',
                $resolvedParam,
                $resolvedParam,
            );
        } else {
            $condition = 'Z_TYPE_P(' . $resolvedParam . ') == ' . $this->getType();
        }

        return new CompiledExpression('bool', $condition, $expression);
    }

    abstract protected function getType();
}
