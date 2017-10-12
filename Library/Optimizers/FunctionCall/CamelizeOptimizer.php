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
 * CamelizeOptimizer
 *
 * Optimizes calls to 'camelize' using internal function
 */
class CamelizeOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) < 1 || count($expression['parameters']) > 2) {
            throw new CompilerException("'camelize' only accepts one or two parameters");
        }

        $delimiter = 'NULL ';
        if (count($expression['parameters']) == 2) {
            if ($expression['parameters'][1]['parameter']['type'] == 'null') {
                unset($expression['parameters'][1]);
            }
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $context->headersManager->add('kernel/string');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (isset($resolvedParams[1])) {
            $delimiter = $resolvedParams[1];
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('zephir_camelize(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $delimiter . ' );');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
