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

/**
 * GetDefinedVars.
 *
 * Optimizes calls to 'get_defined_vars' using internal function
 */
class GetDefinedVarsOptimizer extends FuncGetArgsOptimizer
{
    /**
     * @param CompilationContext $context
     * @param string             $symbol
     *
     * @return void
     */
    protected function printOutput(CompilationContext $context, string $symbol): void
    {
        $context->headersManager->add('kernel/variables');
        $context->codePrinter->output('zephir_get_defined_vars(' . $symbol . ');');
    }
}
