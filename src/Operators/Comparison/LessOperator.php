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

class LessOperator extends ComparisonBaseOperator
{
    protected string $operator = '<';

    protected string $bitOperator = '<';

    protected string $zvalOperator = 'ZEPHIR_LT';

    protected string $zvalLongOperator = 'ZEPHIR_LT_LONG';

    protected string $zvalLongNegOperator = 'ZEPHIR_GT_LONG';

    protected string $zvalDoubleOperator = 'ZEPHIR_LT_DOUBLE';

    protected string $zvalDoubleNegOperator = 'ZEPHIR_GT_DOUBLE';
}
