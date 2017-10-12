<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * GetClassNsOptimizer
 *
 * Optimizes calls to 'get_class_ns' using internal function
 */
class GetClassNsOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        $numberParameters = count($expression['parameters']);
        if ($numberParameters != 1 && $numberParameters != 2) {
            throw new CompilerException("'get_class_ns' only accepts one or two parameters", $expression);
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (!isset($resolvedParams[1])) {
            $context->codePrinter->output('zephir_get_class_ns(' . $symbol . ', ' . $resolvedParams[0] . ', 0 TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_get_class_ns(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ' TSRMLS_CC);');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
