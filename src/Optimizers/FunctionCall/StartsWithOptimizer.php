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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * StartsWithOptimizer.
 *
 * Checks if a string starts with another
 */
class StartsWithOptimizer extends OptimizerAbstract
{
    protected string $zephirMethodStr = 'zephir_start_with_str';
    protected string $zephirMethod    = 'zephir_start_with';

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (
            2 != count($expression['parameters']) &&
            3 != count($expression['parameters'])
        ) {
            return false;
        }

        if ('string' == $expression['parameters'][1]['parameter']['type']) {
            $str = $expression['parameters'][1]['parameter']['value'];
            unset($expression['parameters'][1]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams(
            $expression['parameters'],
            $context,
            $expression
        );

        $context->headersManager->add('kernel/string');

        if (isset($str)) {
            return new CompiledExpression(
                'bool',
                $this->zephirMethodStr . '(' . $resolvedParams[0] . ', SL("' . $str . '"))',
                $expression
            );
        }

        if (2 == count($expression['parameters'])) {
            return new CompiledExpression(
                'bool',
                $this->zephirMethod . '(' . $resolvedParams[0] . ', '
                . $resolvedParams[1] . ', NULL)',
                $expression
            );
        } else {
            return new CompiledExpression(
                'bool',
                $this->zephirMethod . '(' . $resolvedParams[0] . ', '
                . $resolvedParams[1] . ', ' . $resolvedParams[2] . ')',
                $expression
            );
        }
    }
}
