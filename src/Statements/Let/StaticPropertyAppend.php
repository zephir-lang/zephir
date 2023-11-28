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
 * StaticPropertyAppend.
 *
 * Updates object properties dynamically
 */
class StaticPropertyAppend extends StaticPropertyArrayIndex
{
    /**
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @return array
     * @throws Exception
     * @throws ReflectionException
     */
    protected function getExpressions(
        array $statement,
        CompilationContext $compilationContext
    ): array {
        $offsetExpressions[] = 'a';

        return $offsetExpressions;
    }
}
