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

class IdenticalOperator extends ComparisonBaseOperator
{
    protected $commutative = true;

    protected $operator = '==';

    protected $bitOperator = '==';

    protected $zvalOperator = 'ZEPHIR_IS_IDENTICAL';

    protected $zvalLongOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected $zvalLongNegOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected $zvalStringOperator = 'ZEPHIR_IS_STRING_IDENTICAL';

    protected $zvalBoolOperator = 'ZEPHIR_IS_BOOL_IDENTICAL';

    protected $zvalBoolTrueOperator = 'ZEPHIR_IS_TRUE_IDENTICAL';

    protected $zvalBoolFalseOperator = 'ZEPHIR_IS_FALSE_IDENTICAL';
}
