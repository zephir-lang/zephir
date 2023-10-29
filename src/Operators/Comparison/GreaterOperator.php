<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Comparison;

class GreaterOperator extends ComparisonBaseOperator
{
    protected string $operator = '>';

    protected string $bitOperator = '>';

    protected string $zvalOperator = 'ZEPHIR_GT';

    protected string $zvalLongOperator = 'ZEPHIR_GT_LONG';

    protected string $zvalLongNegOperator = 'ZEPHIR_LT_LONG';

    protected string $zvalDoubleOperator = 'ZEPHIR_GT_DOUBLE';

    protected string $zvalDoubleNegOperator = 'ZEPHIR_LT_DOUBLE';
}
