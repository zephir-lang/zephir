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
 * Updates object properties dynamically.
 */
class StaticPropertyArrayIndex extends StaticPropertyAppend
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    protected function getOffsetExpressions(
        array $statement,
        CompilationContext $compilationContext
    ): array {
        $types = ['string', 'int', 'uint', 'ulong', 'long', 'variable'];
        $offsetExpressions = [];
        foreach ($statement['index-expr'] as $indexExpr) {
            $indexExpression = new Expression($indexExpr);
            $resolvedIndex = $indexExpression->compile($compilationContext);
            if (!in_array($resolvedIndex->getType(), $types, true)) {
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
