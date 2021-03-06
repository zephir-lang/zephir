<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Types;

use Zephir\Types;

/**
 * IstringType.
 *
 * Encapsulates built-in methods for the "istring" type
 */
class IstringType extends StringType
{
    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return Types::T_ISTRING;
    }
}
