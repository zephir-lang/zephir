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

/**
 * CreateInstanceParamsOptimizer.
 *
 * Built-in function that creates new instances of objects from its class name passing parameters as an array
 */
class CreateInstanceParamsOptimizer extends CreateInstanceOptimizer
{
    protected string $exceptionMessage = 'This function requires only two parameters';
    protected int    $parameterCount   = 2;
    protected string $zephirMethod     = 'zephir_create_instance_params';


    /**
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function getTempParameters(Call $call, CompilationContext $context): void
    {
        // Not needed here
    }
}
