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
use Zephir\Utils;

/**
 * ExplodeOptimizer
 *
 * Optimizes calls to 'explode' using internal function
 */
class ExplodeOptimizer extends OptimizerAbstract
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
        if (count($expression['parameters']) < 2) {
            throw new CompilerException("'explode' require two parameter");
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        /**
         * Process limit
         */
        $limit = 'LONG_MAX' ;
        $limitOffset = 2;
        if (count($expression['parameters']) == 3 && $expression['parameters'][2]['parameter']['type'] == 'int') {
            $limit = $expression['parameters'][2]['parameter']['value'] . ' ';
            unset($expression['parameters'][2]);
        }

        if ($expression['parameters'][0]['parameter']['type'] == 'string') {
            $str = Utils::addSlashes($expression['parameters'][0]['parameter']['value']);
            unset($expression['parameters'][0]);
            if (count($expression['parameters']) == 2) {
                $limitOffset = 1;
            }
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (isset($resolvedParams[$limitOffset])) {
            $context->headersManager->add('kernel/operators');
            $limit = 'zephir_get_intval(' . $resolvedParams[$limitOffset] . ') ';
        }

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('array');
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($str)) {
            $context->codePrinter->output('zephir_fast_explode_str(' . $symbol . ', SL("' . $str . '"), ' . $resolvedParams[0] . ', ' . $limit . ' TSRMLS_CC);');
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_fast_explode(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $limit . ' TSRMLS_CC);');
        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
