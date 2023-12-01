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
use Zephir\Variable\Variable;

/**
 * UncamelizeOptimizer.
 *
 * Optimizes calls to 'uncamelize' using internal function
 */
class UncamelizeOptimizer extends CamelizeOptimizer
{
    protected string $warningName  = 'uncamelize';
    protected string $zephirMethod = 'zephir_uncamelize';

    /**
     * @param Call               $call
     * @param Variable|null      $symbolVariable
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function symbolVariablePost(
        Call $call,
        ?Variable $symbolVariable,
        CompilationContext $context
    ): void {
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
    }

    /**
     * @param Call               $call
     * @param Variable|null      $symbolVariable
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function symbolVariablePre(
        Call $call,
        ?Variable $symbolVariable,
        CompilationContext $context
    ): void {
        // empty
    }
}
