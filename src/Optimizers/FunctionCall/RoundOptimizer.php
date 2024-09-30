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
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * RoundOptimizer.
 *
 * Optimizes calls to 'round' using internal function
 * parameters float $val [, int $precision = 0 [, int $mode = PHP_ROUND_HALF_UP ]]
 */
class RoundOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) > 4) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/math');
        $symbolVariable->setDynamicTypes('double');
        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        /* Todo: move var return type -> double as with round, floor */
        $symbol = $context->backend->getVariableCode($symbolVariable);

        switch (count($expression['parameters'])) {
            /*
             * Only float $val
             */
            case 1:
                $context->codePrinter->output(
                    'zephir_round(' . $symbol . ', ' . $resolvedParams[0] . ', NULL, NULL);'
                );
                break;

            /*
             * float $val, int $mode
             */
            case 2:
                $context->codePrinter->output(
                    'zephir_round(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', NULL);'
                );
                break;

            default:
                $context->codePrinter->output(
                    'zephir_round(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $resolvedParams[2] . ');'
                );
                break;
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
