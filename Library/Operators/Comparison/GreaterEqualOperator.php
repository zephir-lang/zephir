<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Operators\Comparison;

class GreaterEqualOperator extends ComparisonBaseOperator
{
    protected $_operator = '>=';

    protected $_bitOperator = '>=';

    protected $_zvalOperator = 'ZEPHIR_GE';

    protected $_zvalLongOperator = 'ZEPHIR_GE_LONG';

    protected $_zvalLongNegOperator = 'ZEPHIR_LE_LONG';

    protected $_zvalDoubleOperator = '!ZEPHIR_LT_DOUBLE';

    protected $_zvalDoubleNegOperator = '!ZEPHIR_GT_DOUBLE';
}
