<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Cache\ClassEntryCache;
use Zephir\Cache\FunctionCache;
use Zephir\Cache\MethodCache;
use Zephir\Cache\StaticMethodCache;
use Zephir\Passes\CallGathererPass;

/**
 * CacheManager.
 *
 * Creates and manages function, method and class entries caches
 */
class CacheManager
{
    /**
     * @var FunctionCache
     */
    protected $functionCache;

    /**
     * @var MethodCache
     */
    protected $methodCache;

    /**
     * @var StaticMethodCache
     */
    protected $staticMethodCache;

    /**
     * @var ClassEntryCache
     */
    protected $classEntryCache;

    /**
     * @var CallGathererPass|null
     */
    protected $gatherer;

    /**
     * Sets the CallGathererPass.
     *
     * @param CallGathererPass $gatherer
     */
    public function setGatherer(CallGathererPass $gatherer = null)
    {
        $this->gatherer = $gatherer;
    }

    /**
     * Creates or returns an existing class entry cache.
     *
     * @return ClassEntryCache
     */
    public function getClassEntryCache()
    {
        if (!$this->classEntryCache) {
            $this->classEntryCache = new ClassEntryCache();
        }

        return $this->classEntryCache;
    }

    /**
     * Creates or returns an existing function cache.
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
     * Creates or returns an existing method cache.
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
     * Creates or returns an existing method cache.
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
