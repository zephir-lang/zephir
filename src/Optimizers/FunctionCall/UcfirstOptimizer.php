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
use Zephir\Exception;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * UcfirstOptimizer.
 *
 * Optimizes calls to 'ucfirst' using internal function
 */
class UcfirstOptimizer extends OptimizerAbstract
{
    protected string $zephirMethod = 'zephir_ucfirst';

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return false|CompiledExpression
     * @throws Exception
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (1 != count($expression['parameters'])) {
            return false;
        }

        [$symbolVariable, $resolvedParams, $symbol] = $this->processStringOptimizer(
            $call,
            $context,
            $expression
        );

        $context->codePrinter->output(
            $this->zephirMethod
            . '(' . $symbol . ', ' . $resolvedParams[0] . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
