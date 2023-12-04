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
 * FwriteOptimizer.
 *
 * Optimizes calls to 'fwrite' using internal function
 */
class FwriteOptimizer extends OptimizerAbstract
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

        if (2 != count($expression['parameters'])) {
            return false;
        }

        $context->headersManager->add('kernel/file');

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable();
        $this->checkNotVariableString($symbolVariable, $expression);

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($symbolVariable) {
            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }
            $symbol = $context->backend->getVariableCode($symbolVariable);
            $context->codePrinter->output(
                'zephir_fwrite(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        } else {
            $context->codePrinter->output(
                'zephir_fwrite(NULL, ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
            );
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
