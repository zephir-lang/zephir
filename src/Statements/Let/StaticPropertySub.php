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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;

use function sprintf;

/**
 * StaticPropertySub.
 *
 * Updates static properties
 */
class StaticPropertySub extends StaticPropertyAdd
{
    protected string $methodName = 'subStaticProperty';
}
