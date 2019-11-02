<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * ArrayMergeOptimizer.
 *
 * Optimizes calls to 'array_merge' using internal function
 */
class ArrayMergeOptimizer extends OptimizerAbstract
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

        if (2 != \count($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Returned values by functions can only be assigned to variant variables',
                $expression
            );
        }

        $context->headersManager->add('kernel/array');

        $symbolVariable->setDynamicTypes('array');
        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->codePrinter->output(
            sprintf(
                'zephir_fast_array_merge(%s, %s, %s);',
                $symbol,
                $resolvedParams[0],
                $resolvedParams[1]
            )
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
