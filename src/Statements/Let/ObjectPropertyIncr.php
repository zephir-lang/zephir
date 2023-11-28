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
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

use function current;

/**
 * ObjectPropertyIncr.
 *
 * Increments an object property
 */
class ObjectPropertyIncr extends ObjectPropertyDecr
{
    protected string $zephirMethod = 'zephir_property_incr';
}
