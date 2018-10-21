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

class GreaterOperator extends ComparisonBaseOperator
{
    protected $_operator = '>';

    protected $_bitOperator = '>';

    protected $_zvalOperator = 'ZEPHIR_GT';

    protected $_zvalLongOperator = 'ZEPHIR_GT_LONG';

    protected $_zvalLongNegOperator = 'ZEPHIR_LT_LONG';

    protected $_zvalDoubleOperator = 'ZEPHIR_GT_DOUBLE';

    protected $_zvalDoubleNegOperator = 'ZEPHIR_LT_DOUBLE';
}
