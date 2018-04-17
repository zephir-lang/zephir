<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: https://zephir-lang.com/license.html               |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;
use Zephir\Exception;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * Zephir\Optimizers\FunctionCall\IsPrivatePropertyOptimizer
 *
 * Allows to fastly check if a property has private visibility
 */
class IsPrivatePropertyOptimizer extends OptimizerAbstract
{
    /**
     * @param  array              $expression
     * @param  Call               $call
     * @param  CompilationContext $context
     *
     * @return bool|CompiledExpression
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) != 1) {
            return false;
        }

        $context->headersManager->add('kernel/object');

        try {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
            return new CompiledExpression('bool', 'zephir_is_private_prop(' . $resolvedParams[0] . ')', $expression);
        } catch (Exception $e) {
            throw  new CompilerException($e->getMessage(), $expression);
        }
    }
}
