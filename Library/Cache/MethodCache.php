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
use Zephir\ClassMethod;
use Zephir\Variable;
use Zephir\ClassDefinition;
use Zephir\MethodCallWarmUp;
use Zephir\CompilationContext;
use Zephir\Passes\CallGathererPass;

/**
 * MethodCache
 *
 * Calls in Zephir implement monomorphic and polymorphic caches to
 * improve performance. Method/Functions lookups are cached in a standard
 * first-level method lookup cache.
 *
 * The concept of inline caching is based on the empirical observation
 * that the objects that occur at a particular call site are often of the same type
 * Internal functions are considered monomorphic since they do not change across execution.
 * Final and private methods are also monomorphic because of their own nature.
 * Due to the Ahead-Of-Time compilation approach provided by Zephir, is not possible
 * to implement inline caches, however is possible to add barriers/guards to
 * take advantage of profile guided optimizations (PGO) and branch prediction.
 *
 * This implementation is based on the work of Hölzle, Chambers and Ungar [1].
 *
 * [1] http://www.cs.ucsb.edu/~urs/oocsb/papers/ecoop91.pdf
 */
class MethodCache
{
    protected $cache = array();

    protected $gatherer;

    /**
     * MethodCache
     *
     * @param CallGathererPass $gatherer
     */
    public function __construct(CallGathererPass $gatherer = null)
    {
        $this->gatherer = $gatherer;
    }

    /**
     * Checks if the class is suitable for caching
     *
     * @param ClassDefinition $classDefinition
     * @return boolean
     */
    private function isClassCacheable($classDefinition)
    {
        if ($classDefinition instanceof ClassDefinition) {
            return true;
        }
        if ($classDefinition instanceof \ReflectionClass) {
            if ($classDefinition->isInternal() && $classDefinition->isInstantiable()) {
                $extension = $classDefinition->getExtension();
                switch ($extension->getName()) {
                    case 'Reflection':
                    case 'Core':
                    case 'SPL':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Retrieves/Creates a function cache for a method call
     *
     * @param CompilationContext $compilationContext
     * @param ClassMethod $method
     * @param Variable $caller
     */
    public function get(CompilationContext $compilationContext, $methodName, Variable $caller)
    {
        $compiler = $compilationContext->compiler;

        $numberPoly = 0;

        if ($caller->getRealName() == 'this') {
            $classDefinition = $compilationContext->classDefinition;
            if ($classDefinition->hasMethod($methodName)) {
                $numberPoly++;
                $method = $classDefinition->getMethod($methodName);
            }
        } else {
            $classTypes = $caller->getClassTypes();
            foreach ($classTypes as $classType) {
                if ($compiler->isClass($classType) || $compiler->isInterface($classType) || $compiler->isBundledClass($classType) || $compiler->isBundledInterface($classType)) {
                    if ($compiler->isInterface($classType)) {
                        continue;
                    }

                    if ($compiler->isClass($classType)) {
                        $classDefinition = $compiler->getClassDefinition($classType);
                    } else {
                        $classDefinition = $compiler->getInternalClassDefinition($classType);
                    }

                    if (!$classDefinition) {
                        continue;
                    }

                    if ($classDefinition->hasMethod($methodName) && !$classDefinition->isInterface()) {
                        $numberPoly++;
                        $method = $classDefinition->getMethod($methodName);
                    }
                }
            }
        }

        if (!$numberPoly) {
            // Try to generate a cache based on the fact the variable is not modified within the loop block
            if ($compilationContext->insideCycle && !$caller->isTemporal()) {
                if (count($compilationContext->cycleBlocks) && $caller->getType() == 'variable') {
                    $currentBlock = $compilationContext->cycleBlocks[count($compilationContext->cycleBlocks) - 1];

                    if ($currentBlock->getMutateGatherer(true)->getNumberOfMutations($caller->getName()) == 0) {
                        $functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zephir_fcall_cache_entry', $compilationContext);
                        $functionCache->setMustInitNull(true);
                        $functionCache->setReusable(false);

                        return '&' . $functionCache->getName() . ', 0';
                    }
                }
            }

            return 'NULL, 0';
        }

        if (!($method instanceof \ReflectionMethod)) {
            if ($method->getClassDefinition()->isExternal()) {
                return 'NULL, 0';
            }

            $completeName = $method->getClassDefinition()->getCompleteName();
            if (isset($this->cache[$completeName][$method->getName()])) {
                return $this->cache[$completeName][$method->getName()] . ', ' . SlotsCache::getExistingMethodSlot($method);
            }

            $gatherer = $this->gatherer;
            if (is_object($gatherer)) {
                $number = $gatherer->getNumberOfMethodCalls($method->getClassDefinition()->getCompleteName(), $method->getName());
            } else {
                $number = 0;
            }

            $staticCacheable = !$method->getClassDefinition()->isInterface() && ($compilationContext->currentMethod == $method || $method->getClassDefinition()->isFinal() || $method->isFinal() || $method->isPrivate());
            if ($number > 1 || $compilationContext->insideCycle) {
                $cacheable = true;
            } else {
                $cacheable = false;
            }

        } else {
            $staticCacheable = false;
            $cacheable = false;
        }

        // Recursive methods require warm-up
        if ($compilationContext->currentMethod == $method) {
            if (!$compilationContext->methodWarmUp) {
                $compilationContext->methodWarmUp = new MethodCallWarmUp;
            }
        }

        $associatedClass = false;
        if ($caller->getName() != 'this_ptr') {
            $associatedClass = $caller->getAssociatedClass();
            if ($this->isClassCacheable($associatedClass)) {
                $staticCacheable = true;
            }
        }

        if ($staticCacheable) {
            $cacheSlot = SlotsCache::getMethodSlot($method);
        } else {
            $cacheSlot = '0';
        }

        if ($cacheable) {
            $functionCacheVar = $compilationContext->symbolTable->getTempVariableForWrite('zephir_fcall_cache_entry', $compilationContext);
            $functionCacheVar->setMustInitNull(true);
            $functionCacheVar->setReusable(false);
            $functionCache = '&' . $functionCacheVar->getName();
        } else {
            $functionCache = 'NULL';
        }

        if (!($method instanceof \ReflectionMethod)) {
            $this->cache[$completeName][$method->getName()] = $functionCache;
        }

        return $functionCache . ', ' . $cacheSlot;
    }
}
