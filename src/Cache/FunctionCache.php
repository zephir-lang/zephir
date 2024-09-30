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
use Zephir\Passes\CallGathererPass;

/**
 * Calls in Zephir implement monomorphic and polymorphic caches to
 * improve performance. Method/Functions lookups are cached in a standard
 * first-level method lookup cache.
 *
 * The concept of inline caching is based on the empirical observation
 * that the objects that occur at a particular call site are often of the same type
 * Internal functions are considered monomorphic since they do not change across execution.
 * Final and private methods are also monomorphic because of their own nature.
 * Due to the Ahead-Of-Time (AOT) compilation approach provided by Zephir, is not possible
 * to implement inline caches, however is possible to add barriers/guards to
 * take advantage of profile guided optimizations (PGO) and branch prediction.
 *
 * This implementation is based on the work of Hölzle, Chambers and Ungar [1].
 *
 * [1] http://www.cs.ucsb.edu/~urs/oocsb/papers/ecoop91.pdf
 */
class FunctionCache
{
    protected array $cache = [];

    public function __construct(protected ?CallGathererPass $gatherer = null)
    {
    }

    /**
     * Retrieves/Creates a function cache for a function call.
     */
    public function get(string $functionName, CompilationContext $compilationContext, bool $exists): string
    {
        if (isset($this->cache[$functionName])) {
            return $this->cache[$functionName] . ', ' . SlotsCache::getExistingFunctionSlot($functionName);
        }

        if (!$exists) {
            return 'NULL, 0';
        }

        $cacheSlot = SlotsCache::getFunctionSlot($functionName);

        $number = 0;
        if (!$compilationContext->insideCycle && $this->gatherer !== null) {
            $number = $this->gatherer->getNumberOfFunctionCalls($functionName);
            if ($number <= 1) {
                return 'NULL, ' . $cacheSlot;
            }
        }

        if ($compilationContext->insideCycle || $number > 1) {
            $functionCacheVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'zephir_fcall_cache_entry',
                $compilationContext
            );
            $functionCacheVariable->setMustInitNull(true);
            $functionCacheVariable->setReusable(false);
            $functionCache = '&' . $functionCacheVariable->getName();
        } else {
            $functionCache = 'NULL';
        }

        $this->cache[$functionName] = $functionCache;

        return $functionCache . ', ' . $cacheSlot;
    }
}
