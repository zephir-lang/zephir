<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Cache;

use Zephir\ClassMethod;

/**
 * SlotsCache.
 *
 * In order to reduce memory allocation when calling functions and method_exists
 * Zephir provides a global cache that store pointers to resolved functions
 * that aren't dynamical reducing the time to lookup functions and methods
 */
class SlotsCache
{
    const MAX_SLOTS_NUMBER = 512;
    private static $slot = 1;

    private static $cacheMethodSlots = [];

    private static $cacheFunctionSlots = [];

    /**
     * Returns or creates a cache slot for a function.
     *
     * @param string $functionName
     *
     * @return int
     */
    public static function getFunctionSlot($functionName)
    {
        if (isset(self::$cacheFunctionSlots[$functionName])) {
            return self::$cacheFunctionSlots[$functionName];
        }

        $slot = self::$slot++;
        if ($slot >= self::MAX_SLOTS_NUMBER) {
            return 0;
        }

        self::$cacheFunctionSlots[$functionName] = $slot;

        return $slot;
    }

    /**
     * Returns or creates a cache slot for a function.
     *
     * @param string $functionName
     *
     * @return int
     */
    public static function getExistingFunctionSlot($functionName)
    {
        if (isset(self::$cacheFunctionSlots[$functionName])) {
            return self::$cacheFunctionSlots[$functionName];
        }

        return 0;
    }

    /**
     * Returns or creates a cache slot for a method.
     *
     * @param ClassMethod $method
     *
     * @return int
     */
    public static function getMethodSlot(ClassMethod $method)
    {
        $className = $method->getClassDefinition()->getCompleteName();
        $methodName = $method->getName();

        if (isset(self::$cacheMethodSlots[$className][$methodName])) {
            return self::$cacheMethodSlots[$className][$methodName];
        }

        $slot = self::$slot++;
        if ($slot >= self::MAX_SLOTS_NUMBER) {
            return 0;
        }

        self::$cacheMethodSlots[$className][$methodName] = $slot;

        return $slot;
    }

    /**
     * Returns a cache slot for a method.
     *
     * @param ClassMethod $method
     *
     * @return int
     */
    public static function getExistingMethodSlot(ClassMethod $method)
    {
        $className = $method->getClassDefinition()->getCompleteName();
        $methodName = $method->getName();

        if (isset(self::$cacheMethodSlots[$className][$methodName])) {
            return self::$cacheMethodSlots[$className][$methodName];
        }

        return 0;
    }
}
