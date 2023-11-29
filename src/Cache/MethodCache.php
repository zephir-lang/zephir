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

use ReflectionClass;
use ReflectionException;
use ReflectionMethod;
use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;
use Zephir\Passes\CallGathererPass;
use Zephir\Variable\Variable;

use function count;
use function in_array;
use function is_object;

/**
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
    protected array $cache = [];

    public function __construct(protected ?CallGathererPass $gatherer = null)
    {
    }

    /**
     * Retrieves/Creates a function cache for a method call.
     *
     * @throws ReflectionException
     */
    public function get(CompilationContext $compilationContext, string $methodName, Variable $caller): string
    {
        $compiler = $compilationContext->compiler;

        $numberPoly = 0;

        if ('this' === $caller->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if ($classDefinition->hasMethod($methodName)) {
                ++$numberPoly;
                $method = $classDefinition->getMethod($methodName);
            }
        } else {
            $classTypes = $caller->getClassTypes();
            foreach ($classTypes as $classType) {
                if (
                    $compiler->isClass($classType) ||
                    $compiler->isInterface($classType) ||
                    $compiler->isBundledClass($classType) ||
                    $compiler->isBundledInterface($classType)
                ) {
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
                        ++$numberPoly;
                        $method = $classDefinition->getMethod($methodName);
                    }
                }
            }
        }

        if (!$numberPoly) {
            // Try to generate a cache based on the fact the variable is not modified within the loop block
            if ($compilationContext->insideCycle && !$caller->isTemporal()) {
                if (count($compilationContext->cycleBlocks) && 'variable' == $caller->getType()) {
                    $currentBlock = $compilationContext->cycleBlocks[count($compilationContext->cycleBlocks) - 1];

                    if (0 == $currentBlock->getMutateGatherer(true)->getNumberOfMutations($caller->getName())) {
                        $functionCache = $compilationContext->symbolTable->getTempVariableForWrite(
                            'zephir_fcall_cache_entry',
                            $compilationContext
                        );
                        $functionCache->setMustInitNull(true);
                        $functionCache->setReusable(false);

                        return '&' . $functionCache->getName() . ', 0';
                    }
                }
            }

            return 'NULL, 0';
        }

        if (!($method instanceof ReflectionMethod)) {
            if ($method->getClassDefinition()->isExternal()) {
                return 'NULL, 0';
            }

            $completeName = $method->getClassDefinition()->getCompleteName();
            if (isset($this->cache[$completeName][$method->getName()])) {
                return $this->cache[$completeName][$method->getName()]
                    . ', '
                    . SlotsCache::getExistingMethodSlot($method);
            }

            $gatherer = $this->gatherer;
            if (is_object($gatherer)) {
                $number = $gatherer->getNumberOfMethodCalls(
                    $method->getClassDefinition()->getCompleteName(),
                    $method->getName()
                );
            } else {
                $number = 0;
            }

            $staticCacheable = !$method->getClassDefinition()->isInterface() &&
                (
                    $compilationContext->currentMethod == $method ||
                    $method->getClassDefinition()->isFinal() ||
                    $method->isFinal() ||
                    $method->isPrivate()
                );
            if ($number > 1 || $compilationContext->insideCycle) {
                $cacheable = true;
            } else {
                $cacheable = false;
            }
        } else {
            $staticCacheable = false;
            $cacheable       = false;
        }

        if ('this_ptr' !== $caller->getName()) {
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
            $functionCacheVar = $compilationContext->symbolTable->getTempVariableForWrite(
                'zephir_fcall_cache_entry',
                $compilationContext
            );
            $functionCacheVar->setMustInitNull(true);
            $functionCacheVar->setReusable(false);
            $functionCache = '&' . $functionCacheVar->getName();
        } else {
            $functionCache = 'NULL';
        }

        if (!($method instanceof ReflectionMethod)) {
            $this->cache[$completeName][$method->getName()] = $functionCache;
        }

        return $functionCache . ', ' . $cacheSlot;
    }

    /**
     * Checks if the class is suitable for caching.
     *
     * @param Definition|ReflectionClass|null $classDefinition
     *
     * @return bool
     */
    private function isClassCacheable($classDefinition = null): bool
    {
        if ($classDefinition instanceof Definition) {
            return true;
        }

        if (!($classDefinition instanceof ReflectionClass)) {
            return false;
        }

        return $classDefinition->isInternal()
            && $classDefinition->isInstantiable()
            && in_array($classDefinition->getExtension()->getName(), ['Reflection', 'Core', 'SPL']);
    }
}
