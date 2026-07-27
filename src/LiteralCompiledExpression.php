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

namespace Zephir;

/**
 * LiteralCompiledExpression.
 *
 * This represents a compiled expression as CompiledExpression, but the contents of
 * the resolved code is potentially a simple literal expression
 */
class LiteralCompiledExpression extends CompiledExpression
{
    /**
     * A literal `char`/`uchar` holds the bare character, which is not valid C
     * on its own — quote it as a C character constant.
     */
    public function getCharCode(): string
    {
        return "'" . $this->code . "'";
    }
}
