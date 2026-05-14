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
use Zephir\Variable\Variable;

use function count;

/**
 * UniqueKeyOptimizer.
 *
 * Optimizes calls to 'unique_key' using internal function
 */
class UniqueKeyOptimizer extends OptimizerAbstract
{
    protected string $zephirMethod = 'zephir_unique_key';

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
            throw new CompilerException("'unique_key' only accepts two parameter", $expression);
        }
        return $this->processOptimizer($call, $context, $expression);
    }

    /**
     * @param Call               $call
     * @param CompilationContext $context
     * @param array              $expression
     *
     * @return CompiledExpression
     * @throws Exception
     */
    protected function processOptimizer(Call $call, CompilationContext $context, array $expression): CompiledExpression
    {
        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $this->setHeaders($context, $symbolVariable);

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            $this->zephirMethod
            . '(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param CompilationContext $context
     * @param Variable|null      $symbolVariable
     *
     * @return void
     */
    protected function setHeaders(CompilationContext $context, ?Variable $symbolVariable): void
    {
        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('string');
    }
}
