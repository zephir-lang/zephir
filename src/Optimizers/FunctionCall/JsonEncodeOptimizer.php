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
 * JsonEncodeOptimizer.
 *
 * Optimizes calls to 'json_encode' using internal function
 */
class JsonEncodeOptimizer extends JsonDecodeOptimizer
{
    protected string $zephirMethod = 'zephir_json_encode';

    /**
     * @param Call               $call
     * @param array              $expression
     * @param CompilationContext $context
     *
     * @return Variable|null
     */
    private function processSymbolVariable(
        Call $call,
        array $expression,
        CompilationContext $context
    ): ?Variable {
        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        return $symbolVariable;
    }
}
