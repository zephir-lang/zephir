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

namespace Zephir\Types;

/**
 * Trait for numeric types (int, double) that share common mathematical method mappings
 */
trait NumericTypeTrait
{
    /**
     * Get the common numeric method map shared between int and double types
     */
    protected function getNumericMethodMap(): array
    {
        return [
            'abs'      => 'abs',
            'tobinary' => 'decbin',
            'tohex'    => 'dechex',
            'tooctal'  => 'decoct',
            'pow'      => 'pow',
            'sqrt'     => 'sqrt',
            'exp'      => 'exp',
            'sin'      => 'sin',
            'cos'      => 'cos',
            'tan'      => 'tan',
            'asin'     => 'asin',
            'acos'     => 'acos',
            'atan'     => 'atan',
            'log'      => 'log',
        ];
    }
}
