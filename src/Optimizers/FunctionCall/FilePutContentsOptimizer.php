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
 * FilePutContentsOptimizer.
 *
 * Optimizes calls to 'file_put_contents' using internal function
 */
class FilePutContentsOptimizer extends OptimizerAbstract
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
            $symbol = $context->backend->getVariableCode($symbolVariable);
            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }
            $context->codePrinter->output(
                'zephir_file_put_contents(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        } else {
            $context->codePrinter->output(
                'zephir_file_put_contents(NULL, ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
            );
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
