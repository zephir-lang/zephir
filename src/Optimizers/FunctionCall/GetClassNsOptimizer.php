<?php

declare(strict_types=1);

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
 * GetClassNsOptimizer.
 *
 * Optimizes calls to 'get_class_ns' using internal function
 */
class GetClassNsOptimizer extends OptimizerAbstract
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

        $numberParameters = \count($expression['parameters']);
        if (1 != $numberParameters && 2 != $numberParameters) {
            throw new CompilerException("'get_class_ns' only accepts one or two parameters", $expression);
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
        }

        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (!isset($resolvedParams[1])) {
            $context->codePrinter->output('zephir_get_class_ns('.$symbol.', '.$resolvedParams[0].', 0);');
        } else {
            $context->codePrinter->output('zephir_get_class_ns('.$symbol.', '.$resolvedParams[0].', '.$resolvedParams[1].');');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
