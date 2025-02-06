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

use Zephir\Passes\CallGathererPass;

/**
 * Creates and manages function, method and class entries caches
 */
class Manager
{
    protected ?ClassEntryCache $classEntryCache = null;

    protected ?FunctionCache $functionCache = null;

    protected ?CallGathererPass $gatherer = null;

    protected ?MethodCache $methodCache = null;

    protected ?StaticMethodCache $staticMethodCache = null;

    /**
     * Creates or returns an existing class entry cache.
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
     */
    public function getStaticMethodCache(): ?StaticMethodCache
    {
        if (!$this->staticMethodCache) {
            $this->staticMethodCache = new StaticMethodCache();
        }

        return $this->staticMethodCache;
    }

    /**
     * Sets the CallGathererPass.
     */
    public function setGatherer(?CallGathererPass $gatherer = null): void
    {
        $this->gatherer = $gatherer;
    }
}
