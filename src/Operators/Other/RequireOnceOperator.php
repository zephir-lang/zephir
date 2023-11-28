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

namespace Zephir\Operators\Other;

/**
 * Includes once a plain PHP file
 */
class RequireOnceOperator extends RequireOperator
{
    protected string $operatorName = 'require_once';
    protected string $warningName  = 'non-valid-require-once';
    protected string $zvalName     = 'zephir_require_once_zval';
    protected string $zvalNameRet  = 'zephir_require_once_zval_ret';
}
