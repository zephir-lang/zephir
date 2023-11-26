<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Comparison;

class EqualsOperator extends ComparisonBaseOperator
{
    protected bool $commutative = true;

    protected string $operator = '==';

    protected string $bitOperator = '==';

    protected string $zvalOperator = 'ZEPHIR_IS_EQUAL';

    protected string $zvalLongOperator = 'ZEPHIR_IS_LONG';

    protected string $zvalLongNegOperator = 'ZEPHIR_IS_LONG';

    protected string $zvalStringOperator = 'ZEPHIR_IS_STRING';

    protected string $zvalBoolOperator = 'ZEPHIR_IS_BOOL_VALUE';

    protected string $zvalBoolTrueOperator = 'ZEPHIR_IS_TRUE';

    protected string $zvalBoolFalseOperator = 'ZEPHIR_IS_FALSE';

    protected string $zvalNullOperator = 'ZEPHIR_IS_NULL';
}
