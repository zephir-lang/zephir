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

namespace Zephir;

use Zephir\Cache\FunctionCache;
use Zephir\Cache\MethodCache;
use Zephir\Cache\StaticMethodCache;
use Zephir\Passes\CallGathererPass;

/**
 * CacheManager
 *
 * Creates and manages both function and method caches
 */
class CacheManager
{

    protected $functionCache;

    protected $methodCache;

    protected $staticMethodCache;

    /**
     * Sets the CallGathererPass
     *
     * @param CallGathererPass $gatherer
     */
    public function setGatherer($gatherer)
    {
        $this->gatherer = $gatherer;
    }

    /**
     * Creates or returns an existing function cache
     *
     * @return FunctionCache
     */
    public function getFunctionCache()
    {
        if (!$this->functionCache) {
            $this->functionCache = new FunctionCache($this->gatherer);
        }
        return $this->functionCache;
    }

    /**
     * Creates or returns an existing method cache
     *
     * @return MethodCache
     */
    public function getMethodCache()
    {
        if (!$this->methodCache) {
            $this->methodCache = new MethodCache($this->gatherer);
        }
        return $this->methodCache;
    }

    /**
     * Creates or returns an existing method cache
     *
     * @return StaticMethodCache
     */
    public function getStaticMethodCache()
    {
        if (!$this->staticMethodCache) {
            $this->staticMethodCache = new StaticMethodCache($this->gatherer);
        }
        return $this->staticMethodCache;
    }
}
