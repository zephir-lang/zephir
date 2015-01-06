<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
 | Authors: Rack Lin <racklin@gmail.com>                                    |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * SubstrOptimizer
 *
 * Optimizes calls to 'strpos' using internal function
 */
class SubstrOptimizer extends OptimizerAbstract
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

        /** this optimizer has bugs */
        return false;

        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) < 2) {
            throw new CompilerException("'substr' require two or three parameters");
        }

        /**
         * Process parameters
         */
        $lengthOffset = 2;
        if (isset($expression['parameters'][2]) && $expression['parameters'][2]['parameter']['type'] == 'int') {
            $length = $expression['parameters'][2]['parameter']['value'] . ' ';
            unset($expression['parameters'][2]);
        }

        if (isset($expression['parameters'][1]) && $expression['parameters'][1]['parameter']['type'] == 'int') {
            $from = $expression['parameters'][1]['parameter']['value'] . ' ';
            unset($expression['parameters'][1]);
            $lengthOffset = 1;
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (!isset($length) && isset($resolvedParams[$lengthOffset])) {
            $context->headersManager->add('kernel/operators');
            $length = 'zephir_get_intval(' . $resolvedParams[$lengthOffset] . ') ';
        }

        if (!isset($from) && isset($resolvedParams[1])) {
            $context->headersManager->add('kernel/operators');
            $from = 'zephir_get_intval(' . $resolvedParams[1] . ') ';
        }

        if (!isset($from)) {
            $from = '0 ';
        }
        if (!isset($length)) {
            $length = '0 ';
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

        $context->codePrinter->output('zephir_substr(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ', ' . $from . ', ' . $length .');');
        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
