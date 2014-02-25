<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

/**
 * FunctionCache
 *
 * Calls in Zephir implement monomorphic and polimorphic caches to
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
     *
     */
    public function __construct($gatherer)
    {
        $this->gatherer = $gatherer;
    }

    public function get($functionName, CompilationContext $compilationContext, $exists)
    {

        if (isset($this->cache[$functionName])) {
            return '&' . $this->cache[$functionName]->getName();
        }

        if (!$exists) {
            return 'NULL';
        }

        if (!$compilationContext->insideCycle) {
            $gatherer = $this->gatherer;
            if ($gatherer) {
                $number = $gatherer->getNumberOfFunctionCalls($functionName);
                if ($number <= 1) {
                    return 'NULL';
                }
            } else {
                return 'NULL';
            }
        }

        $functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zephir_fcall_cache_entry', $compilationContext);
        $functionCache->setMustInitNull(true);
        $functionCache->setReusable(false);
        $this->cache[$functionName] = $functionCache;

        return '&' . $functionCache->getName();
    }
}
