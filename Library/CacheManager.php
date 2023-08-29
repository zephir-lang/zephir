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

namespace Zephir;

use Zephir\Cache\ClassEntryCache;
use Zephir\Cache\FunctionCache;
use Zephir\Cache\MethodCache;
use Zephir\Cache\StaticMethodCache;
use Zephir\Passes\CallGathererPass;

/**
 * Creates and manages function, method and class entries caches
 */
class CacheManager
{
    /**
     * @var FunctionCache|null
     */
    protected ?FunctionCache $functionCache = null;

    /**
     * @var MethodCache|null
     */
    protected ?MethodCache $methodCache = null;

    /**
     * @var StaticMethodCache|null
     */
    protected ?StaticMethodCache $staticMethodCache = null;

    /**
     * @var ClassEntryCache|null
     */
    protected ?ClassEntryCache $classEntryCache = null;

    /**
     * @var CallGathererPass|null
     */
    protected ?CallGathererPass $gatherer = null;

    /**
     * Sets the CallGathererPass.
     *
     * @param CallGathererPass|null $gatherer
     */
    public function setGatherer(?CallGathererPass $gatherer = null): void
    {
        $this->gatherer = $gatherer;
    }

    /**
     * Creates or returns an existing class entry cache.
     *
     * @return ClassEntryCache
     */
    public function getClassEntryCache(): ClassEntryCache
    {
        if (!$this->classEntryCache) {
            $this->classEntryCache = new ClassEntryCache();
        }

        return $this->classEntryCache;
    }

    /**
     * Creates or returns an existing function cache.
     *
     * @return FunctionCache|null
     */
    public function getFunctionCache(): ?FunctionCache
    {
        if (!$this->functionCache) {
            $this->functionCache = new FunctionCache($this->gatherer);
        }

        return $this->functionCache;
    }

    /**
     * Creates or returns an existing method cache.
     *
     * @return MethodCache|null
     */
    public function getMethodCache(): ?MethodCache
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
    public function getStaticMethodCache(): ?StaticMethodCache
    {
        if (!$this->staticMethodCache) {
            $this->staticMethodCache = new StaticMethodCache();
        }

        return $this->staticMethodCache;
    }
}
