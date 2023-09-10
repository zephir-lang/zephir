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
 * Encapsulates built-in methods for the "istring" type
 */
class IstringType extends StringType
{
    public function getTypeName(): string
    {
        return Types::T_ISTRING;
    }
}
