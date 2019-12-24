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

class LessOperator extends ComparisonBaseOperator
{
    protected $operator = '<';

    protected $bitOperator = '<';

    protected $zvalOperator = 'ZEPHIR_LT';

    protected $zvalLongOperator = 'ZEPHIR_LT_LONG';

    protected $zvalLongNegOperator = 'ZEPHIR_GT_LONG';

    protected $zvalDoubleOperator = 'ZEPHIR_LT_DOUBLE';

    protected $zvalDoubleNegOperator = 'ZEPHIR_GT_DOUBLE';
}
