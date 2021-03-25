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
 * ExitOptimizer.
 *
 * Optimizes calls to 'exit' using internal function
 */
class ExitOptimizer extends OptimizerAbstract
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
        if (isset($expression['parameters']) && \count($expression['parameters']) > 1) {
            return false;
        }

        $context->headersManager->add('kernel/exit');
        if (isset($expression['parameters'])) {
            //TODO: protect resolvedParams[0] from restore
        }
        if (!isset($expression['parameters'])) {
            $context->codePrinter->output('ZEPHIR_MM_RESTORE();');
            $context->codePrinter->output('zephir_exit_empty();');
        } else {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
            $context->codePrinter->output('ZEPHIR_MM_RESTORE();');
            $context->codePrinter->output('zephir_exit('.$resolvedParams[0].');');
        }

        return new CompiledExpression('void ', '', $expression);
    }
}
