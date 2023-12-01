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
 * GetClassOptimizer.
 *
 * Optimizes calls to 'get_class' using internal function
 */
class GetClassOptimizer extends OptimizerAbstract
{
    protected string $zephirMethod = 'zephir_get_class';

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

        $this->checkParameters($expression);

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->codePrinter->output(
            $this->zephirMethod . '(' . $symbol . ', ' . $resolvedParams[0] . ', 0);'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param array $expression
     *
     * @return void
     */
    protected function checkParameters(array $expression): void
    {
        $numberParameters = count($expression['parameters']);
        if (1 != $numberParameters && 2 != $numberParameters) {
            throw new CompilerException(
                "'get_class' only accepts one or two parameters",
                $expression
            );
        }
    }
}
