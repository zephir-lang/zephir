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
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * InterfaceExistsOptimizer.
 *
 * Optimizes calls to 'interface_exists' using internal function
 */
class InterfaceExistsOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (\count($expression['parameters']) < 1) {
            throw new CompilerException("'class_exists' require one or two parameters");
        }

        /**
         * Process autoload.
         */
        $autoload = '1 ';
        if (2 == \count($expression['parameters']) && ('int' == $expression['parameters'][1]['parameter']['type'] || 'bool' == $expression['parameters'][1]['parameter']['type'])) {
            $autoload = $expression['parameters'][1]['parameter']['value'].' ';
            unset($expression['parameters'][1]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (2 == \count($resolvedParams)) {
            $context->headersManager->add('kernel/operators');
            $autoload = 'zephir_is_true('.$resolvedParams[1].') ';
        }

        $context->headersManager->add('kernel/object');

        return new CompiledExpression('bool', 'zephir_interface_exists('.$resolvedParams[0].', '.$autoload.')', $expression);
    }
}
