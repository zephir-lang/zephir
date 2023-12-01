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
use Zephir\Exception;
use Zephir\Traits\VariablesTrait;

abstract class OptimizerAbstract
{
    use VariablesTrait;

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return mixed
     */
    abstract public function optimize(array $expression, Call $call, CompilationContext $context);

    /**
     * @param Call               $call
     * @param CompilationContext $context
     * @param array              $expression
     *
     * @return array
     * @throws Exception
     */
    protected function processStringOptimizer(Call $call, CompilationContext $context, array $expression): array
    {
        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams(
            $expression['parameters'],
            $context,
            $expression
        );

        $this->symbolVariablePre($call, $symbolVariable, $context);
        $symbol = $context->backend->getVariableCode($symbolVariable);

        return [$symbolVariable, $resolvedParams, $symbol];
    }
}
