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

class IdenticalOperator extends ComparisonBaseOperator
{
    protected $_commutative = true;

    protected $_operator = '==';

    protected $_bitOperator = '==';

    protected $_zvalOperator = 'ZEPHIR_IS_IDENTICAL';

    protected $_zvalLongOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected $_zvalLongNegOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected $_zvalStringOperator = 'ZEPHIR_IS_STRING_IDENTICAL';

    protected $_zvalBoolOperator = 'ZEPHIR_IS_BOOL_IDENTICAL';

    protected $_zvalBoolTrueOperator = 'ZEPHIR_IS_TRUE_IDENTICAL';

    protected $_zvalBoolFalseOperator = 'ZEPHIR_IS_FALSE_IDENTICAL';
}
