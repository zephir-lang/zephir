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
 * InterfaceExistsOptimizer
 *
 * Optimizes calls to 'interface_exists' using internal function
 */
class InterfaceExistsOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) < 1) {
            throw new CompilerException("'class_exists' require one or two parameters");
        }

        /**
         * Process autoload
         */
        $autoload = '1 ';
        if (count($expression['parameters']) == 2 && ($expression['parameters'][1]['parameter']['type'] == 'int' || $expression['parameters'][1]['parameter']['type'] == 'bool')) {
            $autoload = $expression['parameters'][1]['parameter']['value'] . ' ';
            unset($expression['parameters'][1]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (count($resolvedParams) == 2) {
            $context->headersManager->add('kernel/operators');
            $autoload = 'zephir_is_true(' . $resolvedParams[1] . ') ';
        }

        $context->headersManager->add('kernel/object');

        return new CompiledExpression('bool', 'zephir_interface_exists(' . $resolvedParams[0] . ', ' . $autoload . ' TSRMLS_CC)', $expression);
    }
}
