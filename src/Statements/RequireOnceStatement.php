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

namespace Zephir\Statements;

/**
 * Require once statement is used to execute PHP scripts in a given path
 */
class RequireOnceStatement extends RequireStatement
{
    protected string $methodName = 'require_once';
}
