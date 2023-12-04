<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * ClassExistsOptimizer.
 *
 * Optimizes calls to 'class_exists' using internal function
 */
class ClassExistsOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) < 1) {
            throw new CompilerException("'class_exists' require one or two parameters");
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        /*
         * Process autoload
         */
        if (2 == count($resolvedParams)) {
            $context->headersManager->add('kernel/operators');
            $autoload = 'zephir_is_true(' . $resolvedParams[1] . ') ';
        } else {
            $autoload = '1';
        }

        $context->headersManager->add('kernel/object');

        return new CompiledExpression(
            'bool',
            'zephir_class_exists(' . $resolvedParams[0] . ', ' . $autoload . ')',
            $expression
        );
    }
}
