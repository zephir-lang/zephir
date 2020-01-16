<?php

/*
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
    protected $operator = '>';

    protected $bitOperator = '>';

    protected $zvalOperator = 'ZEPHIR_GT';

    protected $zvalLongOperator = 'ZEPHIR_GT_LONG';

    protected $zvalLongNegOperator = 'ZEPHIR_LT_LONG';

    protected $zvalDoubleOperator = 'ZEPHIR_GT_DOUBLE';

    protected $zvalDoubleNegOperator = 'ZEPHIR_LT_DOUBLE';
}
