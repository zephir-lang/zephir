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
 * CreateArrayOptimizer.
 *
 * Built-in function that creates new instances of objects from its class name
 */
class CreateArrayOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters'])) {
            if (count($expression['parameters']) > 1) {
                throw new CompilerException('This function only requires one parameter', $expression);
            }
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $context->headersManager->add('kernel/array');

        $symbolVariable->setDynamicTypes('array');

        if (isset($expression['parameters'])) {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        } else {
            $resolvedParams = null;
        }

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        $symbol = $context->backend->getVariableCode($symbolVariable);
        if ($resolvedParams) {
            $context->codePrinter->output(
                'zephir_create_array(' . $symbol . ', zephir_get_intval(' . $resolvedParams[0] . '), 1);'
            );
        } else {
            $context->codePrinter->output('zephir_create_array(' . $symbol . ', 0, 1);');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
