<?php

/*
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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * Zephir\Optimizers\FunctionCall\IsPrivatePropertyOptimizer.
 *
 * Allows to fastly check if a property has private visibility
 */
class IsPrivatePropertyOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return bool|CompiledExpression
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (1 != \count($expression['parameters'])) {
            return false;
        }

        $context->headersManager->add('kernel/object');

        try {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

            return new CompiledExpression('bool', 'zephir_is_private_prop('.$resolvedParams[0].')', $expression);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression);
        }
    }
}
