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
use Zephir\Optimizers\OptimizerAbstract;

use function array_slice;
use function count;

/**
 * Optimizer for 'call_user_func'.
 *
 * The callback is resolved by the kernel in the class whose code makes the
 * call, as PHP resolves it in a method. Calling PHP's own function instead
 * would resolve it at the nearest PHP frame, the caller of the Zephir method,
 * where a private method of the class is not callable.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class CallUserFuncOptimizer extends OptimizerAbstract
{
    /**
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        $parameters = $expression['parameters'] ?? [];
        if (!$this->acceptsArguments($parameters)) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);
        if (!$symbolVariable) {
            $symbolVariable = $context->symbolTable->addTemp('variable', $context);
            $symbolVariable->initVariant($context);
        }

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $resolvedParams = $call->getReadOnlyResolvedParams($parameters, $context, $expression);

        $context->headersManager->add('kernel/fcall');

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            $this->getOutput($symbol, $resolvedParams, $call->getArgumentNames(), $context)
        );
        $call->addCallStatusOrJump($context);

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }

    /**
     * The callback comes first, by position or as `callback:`.
     */
    protected function acceptsArguments(array $parameters): bool
    {
        return [] !== $parameters && 'callback' === ($parameters[0]['name'] ?? 'callback');
    }

    /**
     * The arguments after the callback go in an array, the named ones under
     * their names, which the engine binds as call_user_func() forwards them.
     *
     * @param string[]               $resolvedParams
     * @param array<int, ?string>    $names
     */
    protected function getOutput(
        string $symbol,
        array $resolvedParams,
        array $names,
        CompilationContext $context
    ): string {
        if (1 === count($resolvedParams)) {
            return 'ZEPHIR_CALL_USER_FUNC(' . $symbol . ', ' . $resolvedParams[0] . ');';
        }

        $arguments = $context->symbolTable->getTempVariableForWrite('variable', $context);
        $context->backend->initArray($arguments, $context, count($resolvedParams) - 1);

        $code = $context->backend->getVariableCode($arguments);
        foreach (array_slice($resolvedParams, 1, null, true) as $position => $param) {
            $context->codePrinter->output(
                null === $names[$position]
                    ? 'zephir_array_fast_append(' . $code . ', ' . $param . ');'
                    : 'zephir_array_update_string(' . $code . ', SL("' . $names[$position] . '"), ' . $param . ', PH_COPY);'
            );
        }

        return 'ZEPHIR_CALL_USER_FUNC_ARGS(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $code . ');';
    }
}
