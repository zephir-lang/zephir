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
 * ArrayKeysOptimizer.
 *
 * Optimizes calls to 'array_keys' using internal function
 */
class ArrayKeysOptimizer extends OptimizerAbstract
{
    protected int $parameterCount = 1;

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     *
     * @return false|CompiledExpression
     * @throws Exception
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if ($this->parameterCount !== count($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        $context->headersManager->add('kernel/array');

        $symbolVariable->setDynamicTypes('array');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->codePrinter->output($this->getOutput($symbol, $resolvedParams));

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param string $symbol
     * @param array  $resolvedParams
     *
     * @return string
     */
    protected function getOutput(string $symbol, $resolvedParams): string
    {
        return sprintf(
            'zephir_array_keys(%s, %s);',
            $symbol,
            $resolvedParams[0]
        );
    }
}
