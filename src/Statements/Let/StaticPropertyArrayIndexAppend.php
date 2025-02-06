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

/**
 * Updates object properties dynamically
 */
class StaticPropertyArrayIndexAppend extends StaticPropertyArrayIndex
{
    /**
     * @throws Exception
     * @throws ReflectionException
     */
    protected function getOffsetExpressions(
        array $statement,
        CompilationContext $compilationContext
    ): array {
        /**
         * Only string/variable/int.
         */
        $offsetExpressions = parent::getOffsetExpressions($statement, $compilationContext);
        $offsetExpressions[] = 'a';

        return $offsetExpressions;
    }
}
