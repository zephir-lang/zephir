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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

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
     * @return bool|CompiledExpression|mixed
     *
     * @throws CompilerException
     * @throws Exception
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) > 1) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/fcall');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        $evalContext = str_replace(
            [getcwd() . '\\', getcwd() . '/'],
            '',
            $expression['file'] . ':' . $expression['line']
        );

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            sprintf('zephir_eval_php(%s, %s, "%s");', $resolvedParams[0], $symbol, $evalContext)
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
