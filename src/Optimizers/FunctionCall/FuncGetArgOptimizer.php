<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
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
 * Zephir\Optimizers\FunctionCall\FuncGetArgOptimizer.
 *
 * Optimizes calls to 'func_get_arg' using internal function.
 */
class FuncGetArgOptimizer extends OptimizerAbstract
{
    /**
     * {@inheritdoc}
     *
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || 1 != \count($expression['parameters'])) {
            throw new CompilerException(
                sprintf(
                    'func_get_arg() expects at exactly 1 parameter, %d given',
                    isset($expression['parameters']) ? \count($expression['parameters']) : 0
                ),
                $expression
            );
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException(
                'Returned values by functions can only be assigned to variant variables.',
                $expression
            );
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        // zephir_get_intval
        $context->headersManager->add('kernel/operators');

        // zephir_get_arg
        $context->headersManager->add('kernel/main');

        $context->codePrinter->output(
            sprintf('zephir_get_arg(%s, zephir_get_intval(%s));', $symbol, $resolvedParams[0])
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
