<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Cache;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Passes\CallGathererPass;

/**
 * FunctionCache
 *
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
    protected $cache = array();

    protected $gatherer;

    /**
     * FunctionCache constructor
     *
     * @param CallGathererPass $gatherer
     */
    public function __construct(CallGathererPass $gatherer = null)
    {
        $this->gatherer = $gatherer;
    }

    /**
     * Checks if a function can be promoted to a static function call
     *
     * @param Call $call
     * @param string $functionName
     * @return boolean
     */
    public function canBeInternal(Call $call, $functionName)
    {
        return true;
    }

    /**
     * Retrieves/Creates a function cache for a function call
     *
     * @param string $functionName
     * @param Call $call
     * @param CompilationContext $compilationContext
     * @param boolean $exists
     */
    public function get($functionName, CompilationContext $compilationContext, Call $call, $exists)
    {
        if (isset($this->cache[$functionName])) {
            return '&' . $this->cache[$functionName]->getName() . ', ' . SlotsCache::getExistingFunctionSlot($functionName);
        }

        if (!$exists) {
            return 'NULL, 0';
        }

        if (!$compilationContext->insideCycle) {
            if (!$this->canBeInternal($call, $functionName)) {
                $gatherer = $this->gatherer;
                if ($gatherer) {
                    $number = $gatherer->getNumberOfFunctionCalls($functionName);
                    if ($number <= 1) {
                        return 'NULL, 0';
                    }
                } else {
                    return 'NULL, 0';
                }
            }
        }

        $functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zephir_fcall_cache_entry', $compilationContext);

        if ($this->canBeInternal($call, $functionName)) {
            $cacheSlot = SlotsCache::getFunctionSlot($functionName);
        } else {
            $cacheSlot = '0';
        }

        $functionCache->setMustInitNull(true);
        $functionCache->setReusable(false);
        $this->cache[$functionName] = $functionCache;

        return '&' . $functionCache->getName() . ', ' . $cacheSlot;
    }
}
