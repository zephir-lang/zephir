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

use Zephir\CompilationContext;
use Zephir\Variable\Variable;

/**
 * JsonEncodeOptimizer.
 *
 * Optimizes calls to 'json_encode' using internal function
 */
class JsonEncodeOptimizer extends JsonDecodeOptimizer
{
    /**
     * @var string
     */
    protected string $zephirMethod = 'zephir_json_encode';


    /**
     * @param Variable|null      $symbolVariable
     * @param CompilationContext $context
     *
     * @return Variable|null
     */
    protected function checkSymbolVariable(
        ?Variable $symbolVariable,
        CompilationContext $context
    ): ?Variable {
        return $symbolVariable;
    }
}
