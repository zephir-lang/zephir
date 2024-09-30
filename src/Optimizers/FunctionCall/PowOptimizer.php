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
use Zephir\Variable\Variable;

use function count;

/**
 * PowOptimizer.
 *
 * Optimizes calls to 'pow' using internal function
 */
class PowOptimizer extends UniqueKeyOptimizer
{
    protected string $zephirMethod = 'zephir_pow_function';

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

        if (2 != count($expression['parameters'])) {
            return false;
        }
    }

    /**
     * @param CompilationContext $context
     * @param Variable|null      $symbolVariable
     *
     * @return void
     */
    protected function setHeaders(CompilationContext $context, ?Variable $symbolVariable): void
    {
        $context->headersManager->add('kernel/operators');
        $symbolVariable->setDynamicTypes('variable');
    }
}
