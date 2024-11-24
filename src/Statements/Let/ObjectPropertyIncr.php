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

/**
 * Increments an object property
 */
class ObjectPropertyIncr extends ObjectPropertyDecr
{
    protected string $zephirMethod = 'zephir_property_incr';
}
