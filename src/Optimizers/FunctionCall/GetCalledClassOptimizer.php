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
 * GetCalledClassOptimizer.
 *
 * Optimizes calls to 'get_called_class' using internal function
 */
class GetCalledClassOptimizer extends OptimizerAbstract
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
        if (isset($expression['parameters'])) {
            $numberParameters = count($expression['parameters']);
            if (0 != $numberParameters) {
                return false;
            }
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('string');

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('zephir_get_called_class(' . $symbol . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
