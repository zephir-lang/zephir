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
 * PregMatchOptimizer
 *
 * Optimizes calls to 'preg_match' using internal function
 */
class PregMatchOptimizer extends OptimizerAbstract
{
    const GLOBAL_MATCH = 0;

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

        if (count($expression['parameters']) < 2 || count($expression['parameters']) > 3) {
            return false;
        }

        /**
         * Process the matches result
         */
        if (isset($expression['parameters'][2]) && $expression['parameters'][2]['parameter']['type'] == 'variable') {
            $matchesVariable = $context->symbolTable->getVariable($expression['parameters'][2]['parameter']['value']);
            if (!$matchesVariable->isInitialized()) {
                $matchesVariable->initVariant($context);
                $matchesVariable->setIsInitialized(true, $context, array());
            }
        } else {
            $matchesVariable = $context->symbolTable->addTemp('variable', $context);
            $matchesVariable->initVariant($context);
        }

        $matchesVariable->setDynamicTypes('array');

        /**
         * Process optional parameters
         */
        $offsetParamOffset = 4;
        if (isset($expression['parameters'][4]) && $expression['parameters'][4]['parameter']['type'] == 'int') {
            $offset = $expression['parameters'][4]['parameter']['value'] . ' ';
            unset($expression['parameters'][4]);
        }

        if (isset($expression['parameters'][3]) && $expression['parameters'][3]['parameter']['type'] == 'int') {
            $flags = $expression['parameters'][3]['parameter']['value'] . ' ';
            unset($expression['parameters'][3]);
            $offsetParamOffset = 3;
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        $context->headersManager->add('kernel/string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (!isset($offset) && isset($resolvedParams[$offsetParamOffset])) {
            $context->headersManager->add('kernel/operators');
            $offset = 'zephir_get_intval(' . $resolvedParams[$offsetParamOffset] . ') ';
        }

        if (!isset($flags) && isset($resolvedParams[3])) {
            $context->headersManager->add('kernel/operators');
            $flags = 'zephir_get_intval(' . $resolvedParams[3] . ') ';
        }

        if (!isset($flags)) {
            $flags = '0 ';
        }
        if (!isset($offset)) {
            $offset = '0 ';
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        $context->codePrinter->output('zephir_preg_match(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $matchesVariable->getName() . ', ' . $this::GLOBAL_MATCH . ', ' . $flags . ', ' . $offset . ' TSRMLS_CC);');
        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
