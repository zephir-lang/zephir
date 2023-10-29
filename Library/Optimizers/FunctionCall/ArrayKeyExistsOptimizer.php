<?php

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
 * ArrayKeysExistsOptimizer.
 *
 * Optimizes calls to 'array_key_exists' using internal function
 */
class ArrayKeyExistsOptimizer extends OptimizerAbstract
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

        if (2 != \count($expression['parameters'])) {
            return false;
        }

        $context->headersManager->add('kernel/array');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        // Note: the first parameter is key in php array_key_exists
        return new CompiledExpression('bool', 'zephir_array_key_exists('.$resolvedParams[1].', '.$resolvedParams[0].')', $expression);
    }
}
