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
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\EvalExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * SqrtOptimizer
 *
 * Optimizes calls to 'sqrt' using internal function
 */
class SqrtOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) > 1) {
            return false;
        }

//        /**
//         * Process the expected symbol to be returned
//         */
//        $call->processExpectedReturn($context);
//
//        $symbolVariable = $call->getSymbolVariable(true, $context);
//
//        if ($symbolVariable->isNotVariable()) {
//            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
//        }
//
//        if ($call->mustInitSymbolVariable()) {
//            $symbolVariable->initVariant($context);
//        }
//
//        $symbolVariable->setDynamicTypes('long');

//        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

//        var_dump($expression['parameters'][0]);

        $resolvedParameter = $expression['parameters'][0]['parameter'];
        if ($resolvedParameter instanceof \Zephir\CompiledExpression) {
            /**
             * @var $resolvedParameter \Zephir\CompiledExpression
             */

//            var_dump($resolvedParameter->getCode());

            return false;
        } else {
            $evalExpression = new EvalExpression();
            $compiledExpression = $evalExpression->optimize($expression['parameters'][0]['parameter'], $context);

            return new CompiledExpression('long', 'sqrt(' . $compiledExpression . ')', $expression);

//            $call->getSymbolVariable(true, $context);

//            $resolvedParameter = $expression['parameters'][0]['parameter']['value'];
//            $context->codePrinter->output(
//                sprintf('%s = abs(%s);', $resolvedParameter, $resolvedParameter)
//            );
        }

//        var_dump($resolvedParameter->getCode());

//        +        $context->codePrinter->output(
//            +            sprintf('%s = abs(%s);', $resolvedParameter, $resolvedParameter)
//            +        );
//        +
//        +        return new CompiledExpression('int', $resolvedParameter, $expression);

//        var_dump($expression);
//        var_dump($expression['parameters'][0]);

//        $evalExpr = new EvalExpression();
//        $eval = $evalExpr->optimize($expression['parameters'][0], $context);

//        $context->codePrinter->output('sqrt(' . $eval . ' TSRMLS_CC);');

        return false;
    }
}
