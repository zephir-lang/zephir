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

namespace Zephir\Statements\Let;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

/**
 * Zephir\Statements\Let\StaticPropertyArrayIndex.
 *
 * Updates object properties dynamically.
 */
class StaticPropertyArrayIndex extends StaticPropertyAppend
{
    /**
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @return array
     * @throws ReflectionException
     * @throws Exception
     */
    protected function getOffsetExpressions(
        array $statement,
        CompilationContext $compilationContext
    ): array {
        /**
         * Only string/variable/int.
         */
        $offsetExpressions = [];
        foreach ($statement['index-expr'] as $indexExpr) {
            $indexExpression = new Expression($indexExpr);

            $resolvedIndex = $indexExpression->compile($compilationContext);
            switch ($resolvedIndex->getType()) {
                case 'string':
                case 'int':
                case 'uint':
                case 'ulong':
                case 'long':
                case 'variable':
                    break;
                default:
                    throw new CompilerException(
                        sprintf(
                            'Expression: %s cannot be used as index without cast',
                            $resolvedIndex->getType()
                        ),
                        $statement['index-expr']
                    );
            }

            $offsetExpressions[] = $resolvedIndex;
        }

        return $offsetExpressions;
    }
}
