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
 * Incr.
 *
 * Increments a variable
 */
class Incr extends Decr
{
    protected string $operator = '++';
    protected string $verb = 'increment';
    protected string $warningName = 'non-valid-increment';
    protected string $zephirMethod = 'zephir_increment';
}
