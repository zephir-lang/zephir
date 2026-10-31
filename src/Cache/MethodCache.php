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

namespace Zephir\Cache;

use Zephir\CompilationContext;

/**
 * Gives each method and static call site whose method name is known at
 * compile time its own inline cache record.
 *
 * The kernel keys a record by the class the method is looked up in and the
 * class of the calling code, which is everything the method PHP runs depends
 * on, so one site seeing objects of several classes stays correct: a class it
 * has not seen last is looked up again. The records of a C function are
 * declared in that function (see Method::compile()) and numbered from 0 there,
 * so a change to one method leaves the generated C of every other one as it
 * was.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2790
 */
class MethodCache
{
    /**
     * Allocates the record of one call site and returns the C expression the
     * `_CACHED` call macros take.
     */
    public function get(CompilationContext $compilationContext): string
    {
        return '&_zephir_call_cache[' . $compilationContext->callCacheCounter++ . ']';
    }
}
