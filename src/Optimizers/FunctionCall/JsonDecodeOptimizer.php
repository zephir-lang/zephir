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
use Zephir\Variable\Variable;

use function count;

/**
 * JsonDecodeOptimizer.
 *
 * Optimizes calls to 'json_decode' using internal function
 */
class JsonDecodeOptimizer extends OptimizerAbstract
{
    protected string $zephirMethod = 'zephir_json_decode';

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

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $this->processSymbolVariable($call, $expression, $context);

        $context->headersManager->add('kernel/string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        /*
         * Process encode options
         */
        if (count($resolvedParams) >= 2) {
            $context->headersManager->add('kernel/operators');
            $options = 'zephir_get_intval(' . $resolvedParams[1] . ') ';
        } else {
            $options = '0 ';
        }

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            $this->zephirMethod
            . '(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $options . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param Call               $call
     * @param array              $expression
     * @param CompilationContext $context
     *
     * @return Variable|null
     */
    private function processSymbolVariable(
        Call $call,
        array $expression,
        CompilationContext $context
    ): ?Variable {
        $symbolVariable = $call->getSymbolVariable();
        $this->checkNotVariable($symbolVariable, $expression);
        if (!$symbolVariable) {
            $symbolVariable = $context->symbolTable->addTemp('variable', $context);
            $symbolVariable->initVariant($context);
        }

        return $symbolVariable;
    }
}
