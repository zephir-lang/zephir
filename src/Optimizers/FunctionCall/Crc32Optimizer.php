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

namespace Zephir\Optimizers\FunctionCall;

/**
 * Crc32Optimizer.
 *
 * Optimizes calls to 'crc32' using internal function
 */
class Crc32Optimizer extends StrtoupperOptimizer
{
    protected string $type         = 'long';
    protected string $zephirMethod = 'zephir_crc32';
}
