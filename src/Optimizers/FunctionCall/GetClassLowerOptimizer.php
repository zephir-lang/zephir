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

use Zephir\Exception\CompilerException;

use function count;

/**
 * GetClassLowerOptimizer.
 *
 * Optimizes calls to 'get_class_lower' using internal function
 */
class GetClassLowerOptimizer extends GetClassOptimizer
{
    /**
     * @param array $expression
     *
     * @return void
     */
    protected function checkParameters(array $expression): void
    {
        $numberParameters = count($expression['parameters']);
        if (1 != $numberParameters) {
            throw new CompilerException(
                "'get_class_lower' only accepts one parameter",
                $expression
            );
        }
    }
}
