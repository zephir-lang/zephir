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
use Zephir\Exception\CompilerException;

use function count;

/**
 * GetClassNsOptimizer.
 *
 * Optimizes calls to 'get_class_ns' using internal function
 */
class GetClassNsOptimizer extends GetClassOptimizer
{
    /**
     * @param array $expression
     *
     * @return void
     */
    protected function checkParameters(array $expression): void
    {
        $numberParameters = count($expression['parameters']);
        if (1 != $numberParameters && 2 != $numberParameters) {
            throw new CompilerException(
                "'get_class_ns' only accepts one or two parameters",
                $expression
            );
        }
    }

    /**
     * @param CompilationContext $context
     * @param string             $symbol
     * @param array              $resolvedParams
     *
     * @return void
     */
    protected function getOutput(
        CompilationContext $context,
        string $symbol,
        array $resolvedParams
    ): void {
        $output = 'zephir_get_class_ns(' . $symbol . ', ' . $resolvedParams[0];

        if (!isset($resolvedParams[1])) {
            $output .=  ', 0);';
        } else {
            $output .=  ', ' . $resolvedParams[1] . ');';
        }

        $context->codePrinter->output($output);
    }
}
