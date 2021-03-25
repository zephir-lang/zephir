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
 * FileGetContentsOptimizer.
 *
 * Optimizes calls to 'file_get_contents' using internal function
 */
class FileGetContentsOptimizer extends OptimizerAbstract
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

        if (1 != \count($expression['parameters'])) {
            return false;
        }

        $context->headersManager->add('kernel/file');

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable) {
            if ($symbolVariable->isNotVariableAndString()) {
                throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
            }
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        if ($symbolVariable) {
            $symbol = $context->backend->getVariableCode($symbolVariable);
            $context->codePrinter->output('zephir_file_get_contents('.$symbol.', '.$resolvedParams[0].');');

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        } else {
            $context->codePrinter->output('zephir_file_get_contents(NULL, '.$resolvedParams[0].');');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
