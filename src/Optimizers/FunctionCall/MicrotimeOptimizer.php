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
 * MicrotimeOptimizer.
 *
 * Optimizes calls to 'microtime' using internal function
 */
class MicrotimeOptimizer extends OptimizerAbstract
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
        /* microtime has one optional parameter (get_as_float) */
        if (isset($expression['parameters']) && count($expression['parameters']) > 2) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/time');

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (!isset($expression['parameters'])) {
            $symbolVariable->setDynamicTypes('string');
            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }
            $context->codePrinter->output('zephir_microtime(' . $symbol . ', NULL);');
        } else {
            $symbolVariable->setDynamicTypes('double');
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }
            $context->codePrinter->output('zephir_microtime(' . $symbol . ', ' . $resolvedParams[0] . ');');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
