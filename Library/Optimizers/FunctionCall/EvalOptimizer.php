<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
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
 * Zephir\Optimizers\FunctionCall\EvalOptimizer.
 */
class EvalOptimizer extends OptimizerAbstract
{
    /**
     * {@inheritdoc}
     *
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     * @throws \Zephir\Exception
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || \count($expression['parameters']) > 1) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException(
                'Returned values by functions can only be assigned to variant variables',
                $expression
            );
        }

        $context->headersManager->add('kernel/fcall');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $evalContext = str_replace(
            [getcwd().'\\', getcwd().'/'],
            '',
            $expression['file'].':'.$expression['line']
        );

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            sprintf('zephir_eval_php(%s, %s, "%s");', $resolvedParams[0], $symbol, $evalContext)
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
